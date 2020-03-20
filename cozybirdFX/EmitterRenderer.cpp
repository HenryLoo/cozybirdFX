#include "EmitterRenderer.h"
#include "AssetLoader.h"
#include "Texture.h"

#include <glad/glad.h>

#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace
{
    const int NUM_PARTICLE_ATTRIBUTES{ 6 };

    const char *PARTICLE_ATTRIBUTES[NUM_PARTICLE_ATTRIBUTES]
    {
       "gsPosition",
       "gsVelocity",
       "gsCurrentLife",
       "gsLife",
       "gsSize",
       "gsType",
    };
}

const int EmitterRenderer::NUM_EMITTERS{ 9 };

EmitterRenderer::EmitterRenderer(AssetLoader &assetLoader)
{    
    // Create update shader program.
    m_updateShader = assetLoader.load<Shader>({ "emitter_update.vs", "", "emitter_update.gs" });
    glTransformFeedbackVaryings(m_updateShader->getProgramId(),
        NUM_PARTICLE_ATTRIBUTES, PARTICLE_ATTRIBUTES, GL_INTERLEAVED_ATTRIBS);
    m_updateShader->link();

    // Create render shader program.
    m_renderShader = assetLoader.load<Shader>({ "emitter_render.vs", "emitter_render.fs", "emitter_render.gs" });
    m_renderShader->link();

    // Calculate axes for billboarding.
    // View vector doesn't change, assuming that camera is fixed.
    m_renderShader->use();
    glm::vec3 viewVec{ 0.f, 0.f, -1.f };
    glm::vec3 upVec{ 0.f, 1.f, 0.f };
    glm::vec3 axis1{ glm::cross(viewVec, upVec) };
    axis1 = glm::normalize(axis1);
    glm::vec3 axis2{ glm::cross(viewVec, axis1) };
    axis2 = glm::normalize(axis2);
    m_renderShader->setVec3("axis1", axis1);
    m_renderShader->setVec3("axis2", axis2);

    // Initialize emitters and start with one emitter enabled.
    init(assetLoader);
}

void EmitterRenderer::render(float deltaTime, const Camera &camera)
{    
    // Skip render if the animation is not playing.
    if (!m_isPlaying)
        return;

    // Ensure that the animation ends at the proper duration if not looping.
    if (!m_isLooping && m_currentTime + deltaTime > m_duration)
        deltaTime = m_duration - m_currentTime;

    // Render all emitters.
    for (int i = 0; i < m_emitters.size(); ++i)
    {
        auto &emitter{ m_emitters[i] };

        if (m_isLooping ||
            (!m_isLooping && m_currentTime < m_duration))
            emitter->update(deltaTime, m_currentTime, m_updateShader);

        // Hide the emitter if not enabled.
        if (m_isEnabled[i])
            emitter->render(camera, m_renderShader);
    }

    // Update the current time.
    if (m_isLooping ||
        (!m_isLooping && m_currentTime < m_duration))
        m_currentTime += deltaTime;

    if (m_isLooping && m_currentTime >= m_duration)
        m_currentTime = 0.f;
}

void EmitterRenderer::init(AssetLoader &assetLoader)
{
    reset();

    m_emitters.clear();
    m_isEnabled.clear();

    // Initialize emitters and start with one emitter enabled.
    for (int i = 0; i < NUM_EMITTERS; ++i)
    {
        auto emitter{ std::make_unique<Emitter>() };
        emitter->setTexture(assetLoader, "particle.png");
        m_emitters.push_back(std::move(emitter));
    }
    m_isEnabled.resize(m_emitters.size());
    m_isEnabled[0] = true;

    m_isPlaying = true;
}

Emitter &EmitterRenderer::getEmitter(int index) const
{
    index = glm::clamp(index, 0, static_cast<int>(m_emitters.size()) - 1);
    return *m_emitters[index];
}

glm::ivec2 EmitterRenderer::getClipSize() const
{
    return m_clipSize;
}

float EmitterRenderer::getCurrentTime() const
{
    return m_currentTime;
}

float EmitterRenderer::getDuration() const
{
	return m_duration;
}

int EmitterRenderer::getExportFPS() const
{
    return m_exportFPS;
}

bool EmitterRenderer::isLooping() const
{
	return m_isLooping;
}

bool EmitterRenderer::isEnabled(int emitterIndex) const
{
    emitterIndex = glm::clamp(emitterIndex, 0, static_cast<int>(m_emitters.size()) - 1);
	return m_isEnabled[emitterIndex];
}

void EmitterRenderer::setClipSize(glm::ivec2 size)
{
    if (size.x > 0)
        m_clipSize.x = size.x;

    if (size.y > 0)
        m_clipSize.y = size.y;
}

void EmitterRenderer::setDuration(float duration)
{
    m_duration = duration;
}

void EmitterRenderer::setExportFPS(int fps)
{
    m_exportFPS = fps;
}

void EmitterRenderer::setLooping(bool isLooping)
{
    m_isLooping = isLooping;
    reset();
}

void EmitterRenderer::setPlaying(bool isPlaying)
{
    m_isPlaying = isPlaying;
    reset();
}

void EmitterRenderer::toggleEmitter(int index, bool isEnabled)
{
    if (m_emitters.empty())
        return;

    index = glm::clamp(index, 0, static_cast<int>(m_emitters.size()) - 1);
    m_isEnabled[index] = isEnabled;
}

void EmitterRenderer::exportSpriteSheet(glm::ivec2 windowSize)
{
    // Get projection matrix.
    glm::mat4 view{ glm::mat4(1.f) };
    glm::vec2 halfClipSize{ m_clipSize.x / 2.f, m_clipSize.y / 2.f };
    glm::mat4 proj{ glm::ortho(
        -halfClipSize.x, halfClipSize.x,
        -halfClipSize.y, halfClipSize.y,
        -1000.f, 1000.f) };

    // View matrix is identity.
    m_renderShader->use();
    m_renderShader->setMat4("mvp", proj);

    // Prepare the framebuffer's texture.
    float fixedDeltaTime{ 1.f / m_exportFPS };
    int numFrames{ static_cast<int>(glm::ceil(m_duration / fixedDeltaTime)) };
    int numCols{ static_cast<int>(glm::ceil(glm::sqrt(numFrames))) };
    int numRows{ static_cast<int>(glm::ceil(numFrames / numCols)) };
    glm::ivec2 textureSize{ numCols * m_clipSize.x, numRows * m_clipSize.y };
    createFramebuffer(textureSize);

    // Bind to the framebuffer to draw to it.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Clear all particles before starting to render.
    for (int i = 0; i < m_emitters.size(); ++i)
    {
        // Skip if disabled.
        if (!m_isEnabled[i])
            continue;

        m_emitters[i]->clear(m_updateShader);
    }

    // Update through the entire duration once to get better loop animation.
    if (m_isLooping)
    {
        m_currentTime = 0.f;
        while (m_currentTime + fixedDeltaTime < m_duration)
        {
            for (int i = 0; i < m_emitters.size(); ++i)
            {
                // Skip if disabled.
                if (!m_isEnabled[i])
                    continue;

                m_emitters[i]->update(fixedDeltaTime, m_currentTime, m_updateShader);
            }

            m_currentTime += fixedDeltaTime;
        }
    }

    // Render the emitters.
    m_currentTime = 0.f;
    for (int j = 0; j < numRows; ++j)
    {
        for (int i = 0; i < numCols; ++i)
        {
            // Ensure that the animation ends at the proper duration if not looping.
            if (!m_isLooping && m_currentTime + fixedDeltaTime > m_duration)
                fixedDeltaTime = m_duration - m_currentTime;

            glViewport(i * m_clipSize.x, textureSize.y - (j + 1) * m_clipSize.y,
                m_clipSize.x, m_clipSize.y);
            //glClear(GL_COLOR_BUFFER_BIT);

            for (int k = 0; k < m_emitters.size(); ++k)
            {
                // Skip if disabled.
                if (!m_isEnabled[k])
                    continue;

                auto &emitter{ m_emitters[k] };
                emitter->update(fixedDeltaTime, m_currentTime, m_updateShader);
                emitter->render(m_renderShader);
                glEnable(GL_RASTERIZER_DISCARD);
            }

            m_currentTime += fixedDeltaTime;
        }
    }

    // Write image from the texture.
    if (m_fboTexture != nullptr)
    {
        m_fboTexture->bind();

        glm::ivec2 size{ m_fboTexture->getWidth(), m_fboTexture->getHeight() };
        int numChannels{ m_fboTexture->getNumChannels() };

        int dataSize{ size.x * size.y * numChannels };
        stbi_uc *data{ new stbi_uc[dataSize] };
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_flip_vertically_on_write(true);
        std::string output{ "output.png" };
        stbi_write_png(output.c_str(), size.x, size.y, numChannels, data, 0);

        delete[] data;
    }

    // Reset configurations after rendering.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowSize.x, windowSize.y);
}

void EmitterRenderer::createFramebuffer(glm::ivec2 textureSize)
{
    // Delete any existing old buffers.
    if (m_fbo) glDeleteFramebuffers(1, &m_fbo);

    // Set up buffers for rendering to texture.
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    unsigned int outputTexture;
    glGenTextures(1, &outputTexture);
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);
    m_fboTexture = std::make_shared<Texture>(outputTexture, textureSize.x, textureSize.y, 4);

    // Check if the framebuffer is complete.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "EmitterRenderer::createFramebuffer: Framebuffer not complete." << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EmitterRenderer::reset()
{
    m_currentTime = 0.f;

    for (auto &emitter : m_emitters)
    {
        emitter->clear(m_updateShader);
    }
}

#include "EmitterRenderer.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "Texture.h"

#include <glad/glad.h>

#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <gif.h>

namespace
{
    const int NUM_PARTICLE_ATTRIBUTES{ 4 };

    const char *PARTICLE_ATTRIBUTES[NUM_PARTICLE_ATTRIBUTES]
    {
       "gsPosition",
       "gsSpeedDirection",
       "gsRotationSizeLife",
       "gsType",
    };

    const std::string SHEET_FORMAT{ ".png" };
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

    // Get camera matrices.
    glm::mat4 view{ camera.getView() };
    glm::mat4 proj{ camera.getSceneProjection() };
    glm::mat4 mvp = proj * view;
    m_renderShader->use();
    m_renderShader->setMat4("mvp", mvp);

    render(deltaTime);
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
        emitter->setTexture(assetLoader, "circle.png");
        m_emitters.push_back(std::move(emitter));
    }
    m_isEnabled.resize(m_emitters.size());
    m_isEnabled[0] = true;

    // Set default values.
    //m_duration = 3.f;
    m_clipSize = glm::ivec2(200.f, 200.f);
    m_exportFPS = 60;
    m_isLooping = true;
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
    // Duration needs to bound the largest max particle life of all emitters,
    // while also being a multiple of timeToSpawn for that emitter.
    float duration{ 0.f };
    float timeToSpawn{ 1.f };
    for (int i = 0; i < m_emitters.size(); ++i)
    {
        if (!m_isEnabled[i])
            continue;

        float maxLife{ m_emitters[i]->getLife().y };
        if (!m_isLooping)
        {
            float tts{ m_emitters[i]->getTimeToSpawn() };
            float emDuration{ m_emitters[i]->getEmitterDuration() };
            float maxDuration{ emDuration == 0.f ? maxLife * 2 : 
                maxLife + tts * glm::floor(emDuration / tts) };
            duration = glm::max(duration, maxDuration);
        }
        else
        {
            duration = glm::max(duration, maxLife);
            timeToSpawn = glm::max(timeToSpawn, m_emitters[i]->getTimeToSpawn());
        }
    }

    if (m_isLooping)
        duration = timeToSpawn * glm::ceil(duration / timeToSpawn);

    return duration;
}

int EmitterRenderer::getExportFPS() const
{
    return m_exportFPS;
}

bool EmitterRenderer::isLooping() const
{
	return m_isLooping;
}

bool EmitterRenderer::isPlaying() const
{
    return m_isPlaying;
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

void EmitterRenderer::exportSpriteSheet(glm::ivec2 windowSize, 
    const std::string &outputPath)
{
    // Prepare the framebuffer's texture.
    float fixedDeltaTime{ getFixedDeltaTime() };
    int numFrames{ static_cast<int>(glm::ceil(getDuration() / fixedDeltaTime)) };
    int numCols{ static_cast<int>(glm::ceil(glm::sqrt(numFrames))) };
    int numRows{ static_cast<int>(glm::ceil(numFrames / numCols)) };
    glm::ivec2 textureSize{ numCols * m_clipSize.x, numRows * m_clipSize.y };
    prepareExport(textureSize, true);

    // Render the emitters.
    for (int j = 0; j < numRows; ++j)
    {
        for (int i = 0; i < numCols; ++i)
        {
            glViewport(i * m_clipSize.x, j * m_clipSize.y,
                m_clipSize.x, m_clipSize.y);

            render(fixedDeltaTime, false, true);
        }
    }

    // Write image from the texture.
    int textureIndex{ 0 };
    for (auto texture : m_fboTexture)
    {
        if (texture == nullptr)
            continue;

        texture->bind();

        int numChannels{ texture->getNumChannels() };
        int dataSize{ textureSize.x * textureSize.y * numChannels };
        stbi_uc *data{ new stbi_uc[dataSize] };
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        const std::string path{ m_fboTexture.size() > 1 ? 
            outputPath.substr(0, outputPath.length() - SHEET_FORMAT.length()) + 
            "_" + std::to_string(textureIndex) + SHEET_FORMAT :
            outputPath };
        stbi_write_png(path.c_str(), textureSize.x, textureSize.y, numChannels, data, 0);
        ++textureIndex;

        delete[] data;
    }

    // Reset configurations after rendering.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowSize.x, windowSize.y);
    m_currentTime = 0.f;
}

void EmitterRenderer::exportGif(glm::ivec2 windowSize, 
    const std::string &outputPath)
{
    prepareExport(m_clipSize, false);

    // Initialize gif writer.
    const int DELAY{ static_cast<int>(round(1.f / m_exportFPS * 100)) };
    GifWriter gif;
    GifBegin(&gif, outputPath.c_str(), m_clipSize.x, m_clipSize.y, DELAY);

    // Render the emitters.
    glViewport(0, 0, m_clipSize.x, m_clipSize.y);
    float fixedDeltaTime{ getFixedDeltaTime() };
    int numFrames{ static_cast<int>(glm::ceil(getDuration() / fixedDeltaTime)) };
    for (int i = 0; i < numFrames; ++i)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        render(fixedDeltaTime, false);

        // Output this gif frame.
        if (m_fboTexture[0] != nullptr)
        {
            m_fboTexture[0]->bind();

            int numChannels{ m_fboTexture[0]->getNumChannels() };
            int dataSize{ m_clipSize.x * m_clipSize.y * numChannels };
            stbi_uc *data{ new stbi_uc[dataSize] };
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            GifWriteFrame(&gif, data, m_clipSize.x, m_clipSize.y, DELAY);

            delete[] data;

            // Prepare the next frame's texture if this isn't the last frame.
            if (i < numFrames - 1)
            {
                // Only use the first texture.
                m_fboTexture.clear();
                createFramebufferTexture(m_clipSize);
            }
        }
    }

    // Reset configurations after rendering.
    GifEnd(&gif);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowSize.x, windowSize.y);
    m_currentTime = 0.f;
}

void EmitterRenderer::createFramebuffers(glm::ivec2 textureSize, bool hasLayers)
{
    clearFramebuffers();

    Emitter::BlendMode lastMode{ Emitter::BlendMode::None };
    for (int i = 0; i < m_emitters.size(); ++i)
    {
        auto &emitter{ m_emitters[i] };

        // Hide the emitter if not enabled.
        if (m_isEnabled[i])
        {
            // Create a new framebuffer if this emitter uses a different blend
            // mode than the previous emitter.
            Emitter::BlendMode thisMode{ emitter->getBlendMode() };
            if (lastMode == thisMode)
                continue;

            lastMode = thisMode;

            createFramebuffer(textureSize, false);

            // Clear to black if additive blending.
            if (thisMode == Emitter::BlendMode::Additive)
                glClear(GL_COLOR_BUFFER_BIT);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EmitterRenderer::createFramebuffer(glm::ivec2 textureSize, bool isClearBuffers)
{
    if (isClearBuffers)
        clearFramebuffers();

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    m_fbo.push_back(fbo);

    createFramebufferTexture(textureSize);

    // Check if the framebuffer is complete.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "EmitterRenderer::createFramebuffer: Framebuffer not complete." << std::endl;
    }
}

void EmitterRenderer::createFramebufferTexture(glm::ivec2 textureSize)
{
    unsigned int outputTexture;
    glGenTextures(1, &outputTexture);
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);
    m_fboTexture.push_back(std::make_shared<Texture>(outputTexture, textureSize.x, textureSize.y, 4));
}

void EmitterRenderer::clearFramebuffers()
{
    // Delete any existing old buffers.
    for (unsigned int &fbo : m_fbo)
        glDeleteFramebuffers(1, &fbo);
    m_fbo.clear();
    m_fboTexture.clear();
}

void EmitterRenderer::reset()
{
    m_currentTime = 0.f;

    for (auto &emitter : m_emitters)
    {
        emitter->clear(m_updateShader);
    }
}

void EmitterRenderer::prepareExport(glm::ivec2 textureSize, bool hasLayers)
{
    // Get projection matrix.
    glm::mat4 view{ glm::mat4(1.f) };
    glm::vec2 halfClipSize{ m_clipSize.x / 2.f, m_clipSize.y / 2.f };
    glm::mat4 proj{ glm::ortho(
        -halfClipSize.x, halfClipSize.x,
        -halfClipSize.y, halfClipSize.y,
        -1000.f, 1000.f) };

    // Flip vertically on output to fix OpenGL's flipped vertical axis.
    proj = glm::scale(proj, glm::vec3(1, -1, 1));

    // View matrix is identity.
    m_renderShader->use();
    m_renderShader->setMat4("mvp", proj);

    // Prepare the framebuffer's texture.
    float fixedDeltaTime{ getFixedDeltaTime() };
    if (hasLayers)
        createFramebuffers(textureSize, hasLayers);
    else
        createFramebuffer(textureSize, true);

    // Clear all particles before starting to render.
    for (int i = 0; i < m_emitters.size(); ++i)
    {
        // Skip if disabled.
        if (!m_isEnabled[i])
            continue;

        m_emitters[i]->clear(m_updateShader);
    }

    // Update through the entire duration once to get better loop animation.
    m_currentTime = 0.f;
    if (m_isLooping)
    {
        while (m_currentTime + fixedDeltaTime < getDuration())
        {
            render(fixedDeltaTime, true, hasLayers);
        }

        m_currentTime = 0.f;
    }
}

void EmitterRenderer::render(float deltaTime, bool isOnlyUpdate, 
    bool hasLayers)
{
    // Ensure that the animation ends at the proper duration if not looping.
    float duration{ getDuration() };
    if (duration == 0.f)
    {
        m_currentTime = 0.f;
        return;
    }

    // Render all emitters.
    int fboIndex{ hasLayers ? -1 : 0 };
    Emitter::BlendMode lastMode{ Emitter::BlendMode::None };
    for (int i = 0; i < m_emitters.size(); ++i)
    {
        auto &emitter{ m_emitters[i] };

        if (m_isLooping ||
            (!m_isLooping && m_currentTime < duration))
            emitter->update(deltaTime, m_currentTime, m_updateShader, m_isLooping);

        // Hide the emitter if not enabled.
        if (!isOnlyUpdate && m_isEnabled[i])
        {
            if (hasLayers)
            {
                // Bind to the next framebuffer if this emitter uses a different blend
                // mode than the previous emitter.
                Emitter::BlendMode thisMode{ emitter->getBlendMode() };
                if (lastMode != thisMode)
                {
                    lastMode = thisMode;
                    ++fboIndex;
                }

                glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[fboIndex]);
            }
            
            emitter->render(m_renderShader);
        }
    }
    
    m_currentTime += deltaTime;

    if (m_currentTime >= duration)
        m_currentTime -= duration;
}

float EmitterRenderer::getFixedDeltaTime() const
{
    return 1.f / m_exportFPS;
}

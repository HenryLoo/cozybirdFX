#include "Emitter.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

#include <iostream>

namespace
{
    const int NUM_PARTICLE_ATTRIBUTES{ 6 };

    const char *PARTICLE_ATTRIBUTES[NUM_PARTICLE_ATTRIBUTES]
    {
       "gsPosition",
       "gsVelocity",
       "gsColour",
       "gsDuration",
       "gsSize",
       "gsType",
    };

    const int MAX_PARTICLES{ 1000 };
}

Emitter::Emitter()
{
    //// Create update shader program.
    //m_updateShader = assetLoader->load<Shader>({ "emitter_update.vs", "", "emitter_update.gs" });
    //glTransformFeedbackVaryings(m_updateShader->getProgramId(),
    //    NUM_PARTICLE_ATTRIBUTES, PARTICLE_ATTRIBUTES, GL_INTERLEAVED_ATTRIBS);
    //m_updateShader->link();

    //// Create render shader program.
    //m_renderShader = assetLoader->load<Shader>({ "emitter_render.vs", "emitter_render.fs", "emitter_render.gs" });
    //m_renderShader->link();

    // Calculate axes for billboarding.
    // View vector doesn't change, assuming that camera is fixed.
    //m_renderShader->use();
    //glm::vec3 viewVec{ 0.f, 0.f, -1.f };
    //glm::vec3 upVec{ 0.f, 1.f, 0.f };
    //glm::vec3 axis1{ glm::cross(viewVec, upVec) };
    //axis1 = glm::normalize(axis1);
    //glm::vec3 axis2{ glm::cross(viewVec, axis1) };
    //axis2 = glm::normalize(axis2);
    //m_renderShader->setVec3("axis1", axis1);
    //m_renderShader->setVec3("axis2", axis2);

    //glGenTransformFeedbacks(1, &m_transFeedbackBuffer);
    glGenBuffers(1, &m_transFeedbackBuffer);
    glGenQueries(1, &m_query);

    glGenBuffers(2, m_particleBuffers);
    glGenVertexArrays(2, m_vao);

    Particle particle;
    particle.type = ParticleType::TYPE_EMITTER;

    for (int i = 0; i < 2; ++i)
    {
        glBindVertexArray(m_vao[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * MAX_PARTICLES, NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle), &particle);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)0);

        // Velocity
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)(2 * sizeof(float)));

        // Colour
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)(4 * sizeof(float)));

        // Duration
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)(7 * sizeof(float)));

        // Size
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)(8 * sizeof(float)));

        // Type
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(Particle), (void *)(9 * sizeof(float)));
    }
}

void Emitter::update(float deltaTime, std::shared_ptr<Shader> updateShader)
{
    updateShader->use();

    // Set emitter uniforms for the update shader.
    updateShader->setFloat("deltaTime", deltaTime);
    updateShader->setVec2("emPos", m_position);
    updateShader->setVec2("emVelocityMin", m_velocityMin);
    updateShader->setVec2("emVelocityOffset", m_velocityOffset);
    updateShader->setVec2("emAcceleration", m_acceleration);
    updateShader->setVec3("emColour", m_colour);
    updateShader->setFloat("emDurationMin", m_durationMin);
    updateShader->setFloat("emDurationOffset", m_durationOffset);
    updateShader->setFloat("emSize", m_size);

    // Set the seed.
    //std::random_device randdev;
    //std::mt19937 generator(randdev());
    std::mt19937 generator(static_cast<unsigned int>(m_currentTime));
    m_currentTime += deltaTime;
    if (m_currentTime >= m_emitterDuration)
        m_currentTime = 0.f;
    std::uniform_real_distribution<> distrib(-1, 1);
    updateShader->setFloat("randomSeed", static_cast<float>(distrib(generator)));

    // Update spawn timer and flag the emitter to spawn particles if necessary.
    m_spawnTimer += deltaTime;
    if (m_spawnTimer >= m_timeToSpawn)
    {
        m_spawnTimer -= m_timeToSpawn;
        updateShader->setInt("emNumToGenerate", m_numToGenerate);
    }
    else
    {
        updateShader->setInt("emNumToGenerate", 0);
    }

    // Disable rasterization because there is no graphical output when 
    // updating particles.
    glEnable(GL_RASTERIZER_DISCARD);

    // Set up transform feedback.
    //glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transFeedbackBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_transFeedbackBuffer);
    glBindVertexArray(m_vao[m_currentReadBuffer]);

    // Enable velocity attribute when updating.
    glEnableVertexAttribArray(1);

    // Set the buffer to store the result of transform feedback.
    int writeBuffer{ 1 - m_currentReadBuffer };
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffers[writeBuffer]);

    // Set up the query.
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_query);

    // Perform transform feedback.
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, m_numParticles);
    glEndTransformFeedback();

    // Stop the query and get its results (the number of generated particles).
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    glGetQueryObjectiv(m_query, GL_QUERY_RESULT, &m_numParticles);

    // Alternate between buffers to use as the read buffer.
    m_currentReadBuffer = 1 - m_currentReadBuffer;

    // Unbind the transform feedback object.
    //glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Emitter::render(Camera *camera, std::shared_ptr<Shader> renderShader)
{
    if (camera == nullptr)
        return;

    // Get camera matrices.
    glm::mat4 view{ camera->getView() };
    glm::mat4 proj{ camera->getSceneProjection() };
    glm::mat4 mvp = proj * view;
    renderShader->use();
    renderShader->setMat4("mvp", mvp);

    // Render the particles.
    render(renderShader);
}

void Emitter::clear()
{
}

int Emitter::getNumParticles() const
{
    return m_numParticles;
}

Texture *Emitter::getOutputTexture() const
{
    return m_outputTexture.get();
}

void Emitter::setTexture(std::shared_ptr<Texture> texture)
{
    m_texture = texture;
}

void Emitter::setNumToGenerate(int num)
{
    m_numToGenerate = num;
}

void Emitter::setPosition(glm::vec2 position)
{
    m_position = position;
}

void Emitter::setTimeToSpawn(float duration)
{
    m_timeToSpawn = duration;
}

void Emitter::setVelocityMin(glm::vec2 velocity)
{
    m_velocityMin = velocity;
}

void Emitter::setVelocityOffset(glm::vec2 velocity)
{
    m_velocityOffset = velocity;
}

void Emitter::setAcceleration(glm::vec2 acceleration)
{
    m_acceleration = acceleration;
}

void Emitter::setSize(float size)
{
    m_size = size;
}

void Emitter::setColour(glm::vec3 colour)
{
    m_colour = colour;
}

void Emitter::setDurationMin(float duration)
{
    m_durationMin = duration;
}

void Emitter::setDurationOffset(float duration)
{
    m_durationOffset = duration;
}

int Emitter::getNumToGenerate() const
{
	return m_numToGenerate;
}

glm::vec2 Emitter::getPosition() const
{
    return m_position;
}

float Emitter::getTimeToSpawn() const
{
    return m_timeToSpawn;
}

glm::vec2 Emitter::getVelocityMin() const
{
    return m_velocityMin;
}

glm::vec2 Emitter::getVelocityOffset() const
{
    return m_velocityOffset;
}

glm::vec2 Emitter::getAcceleration() const
{
    return m_acceleration;
}

float Emitter::getSize() const
{
    return m_size;
}

glm::vec3 Emitter::getColour() const
{
    return m_colour;
}

float Emitter::getDurationMin() const
{
    return m_durationMin;
}

float Emitter::getDurationOffset() const
{
    return m_durationOffset;
}

void Emitter::createFramebuffer(glm::ivec2 textureSize)
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
    m_outputTexture = std::make_shared<Texture>(outputTexture, textureSize.x, textureSize.y, 4);

    // Check if the framebuffer is complete.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Emitter::createFramebuffer: Framebuffer not complete." << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Emitter::outputToTexture(std::shared_ptr<Shader> updateShader, 
    std::shared_ptr<Shader> renderShader)
{
    // Get projection matrix.
    glm::mat4 view{ glm::mat4(1.f) };
    glm::vec2 halfClipSize{ m_clipSize.x / 2.f, m_clipSize.y / 2.f };
    glm::mat4 proj{ glm::ortho(
        -halfClipSize.x, halfClipSize.x,
        -halfClipSize.y, halfClipSize.y,
        -1000.f, 1000.f) };

    // View matrix is identity.
    renderShader->use();
    renderShader->setMat4("mvp", proj);

    // Prepare the framebuffer's texture.
    float fixedDeltaTime{ 1 / 24.f };
    int numFrames{ static_cast<int>(glm::ceil(m_emitterDuration / fixedDeltaTime)) };
    int numCols{ static_cast<int>(glm::ceil(glm::sqrt(numFrames))) };
    int numRows{ static_cast<int>(glm::ceil(numFrames / numCols)) };
    glm::ivec2 textureSize{ numCols * m_clipSize.x, numRows * m_clipSize.y };
    createFramebuffer(textureSize);

    // Bind to the framebuffer to draw to it.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Update through the entire duration once to get better loop animation.
    // TODO: Only do this if the animation should loop.
    m_currentTime = 0.f;
    while (m_currentTime + fixedDeltaTime < m_emitterDuration)
        update(fixedDeltaTime, updateShader);

    // Render the particles.
    m_currentTime = 0.f;
    for (int j = 0; j < numRows; ++j)
    {
        for (int i = 0; i < numCols; ++i)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(i * m_clipSize.x, textureSize.y - (j + 1) * m_clipSize.y,
                m_clipSize.x, m_clipSize.y);
            render(renderShader);

            update(fixedDeltaTime, updateShader);
        }
    }

    // Reset configurations after rendering.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Emitter::render(std::shared_ptr<Shader> renderShader)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Disable writing to depth buffer.
    glDepthMask(0);

    // Enable rasterizer for graphical output.
    glDisable(GL_RASTERIZER_DISCARD);
    renderShader->use();

    // Bind to the read buffer's vertex array object.
    glBindVertexArray(m_vao[m_currentReadBuffer]);

    // Disable velocity attribute since it isn't used for rendering.
    glDisableVertexAttribArray(1);

    // Draw the particles.
    m_texture->bind();
    glDrawArrays(GL_POINTS, 0, m_numParticles);

    // Reset configurations after rendering.
    glDepthMask(1);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}
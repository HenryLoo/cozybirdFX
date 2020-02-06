#include "Emitter.h"
#include "AssetLoader.h"
#include "Shader.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <random>

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

Emitter::Emitter(AssetLoader *assetLoader)
{
    // Create update shader program.
	m_updateShader = assetLoader->load<Shader>({ "emitter_update.vs", "", "emitter_update.gs" });
    glTransformFeedbackVaryings(m_updateShader->getProgramId(), 
            NUM_PARTICLE_ATTRIBUTES, PARTICLE_ATTRIBUTES, GL_INTERLEAVED_ATTRIBS);
	m_updateShader->link();

    // Create render shader program.
    m_renderShader = assetLoader->load<Shader>({ "emitter_render.vs", "emitter_render.fs", "emitter_render.gs" });
    m_renderShader->link();

    glGenTransformFeedbacks(1, &m_transFeedbackBuffer);
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

void Emitter::update(float deltaTime)
{
    m_updateShader->use();

    // Set emitter uniforms for the update shader.
    m_updateShader->setFloat("deltaTime", deltaTime);
    m_updateShader->setVec2("emPos", m_position);
    m_updateShader->setVec2("emVelocityMin", m_velocityMin);
    m_updateShader->setVec2("emVelocityOffset", m_velocityOffset);
    m_updateShader->setVec2("emAcceleration", m_acceleration);
    m_updateShader->setVec3("emColour", m_colour);
    m_updateShader->setFloat("emDurationMin", m_durationMin);
    m_updateShader->setFloat("emDurationOffset", m_durationOffset);
    m_updateShader->setFloat("emSize", m_size);

    // Set the seed.
    std::random_device randdev;
    std::mt19937 generator(randdev());
    std::uniform_real_distribution<> distrib(-1, 1);
    m_updateShader->setFloat("randomSeed", static_cast<float>(distrib(generator)));

    // Update spawn timer and flag the emitter to spawn particles if necessary.
    m_spawnTimer += deltaTime;
    if (m_spawnTimer >= m_timeToSpawn)
    {
        m_spawnTimer -= m_timeToSpawn;
        m_updateShader->setInt("emNumToGenerate", m_numToGenerate);
    }
    else
    {
        m_updateShader->setInt("emNumToGenerate", 0);
    }

    // Disable rasterization because there is no graphical output when 
    // updating particles.
    glEnable(GL_RASTERIZER_DISCARD);

    // Set up transform feedback.
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transFeedbackBuffer);
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
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void Emitter::render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    // Disable writing to depth buffer.
    glDepthMask(0);

    // Enable rasterizer for graphical output.
    glDisable(GL_RASTERIZER_DISCARD);
    m_renderShader->use();

    // TODO: Replace this with something more flexible.
    glm::mat4 view{ glm::mat4(1.0f) };
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 proj{ glm::perspective(glm::radians(45.0f),
        (float)1280 / 720, 0.1f, 100.0f) };
    glm::mat4 mvp = proj * view;
    m_renderShader->setMat4("mvp", mvp);

    glm::vec3 viewVec{ 0.f, 0.f, -1.f };
    glm::vec3 upVec{ 0.f, 1.f, 0.f };
    glm::vec3 axis1{ glm::cross(viewVec, upVec) };
    axis1 = glm::normalize(axis1);
    glm::vec3 axis2{ glm::cross(viewVec, axis1) };
    axis2 = glm::normalize(axis2);
    m_renderShader->setVec3("axis1", axis1);
    m_renderShader->setVec3("axis2", axis2);

    glBindVertexArray(m_vao[m_currentReadBuffer]);

    // Disable velocity attribute since it isn't used for rendering.
    glDisableVertexAttribArray(1);

    // Render the particles.
    m_texture->bind();
    glDrawArrays(GL_POINTS, 0, m_numParticles);

    // Reset configurations after rendering.
    glDepthMask(1);
    glDisable(GL_BLEND);
}

void Emitter::clear()
{
}

int Emitter::getNumParticles() const
{
    return m_numParticles;
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

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
    const int MAX_PARTICLES{ 1000 };
}

Emitter::Emitter()
{
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

        // Current particle life
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)(4 * sizeof(float)));

        // Total particle life
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)(5 * sizeof(float)));

        // Size
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)(6 * sizeof(float)));

        // Type
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(Particle), (void *)(7 * sizeof(float)));
    }
}

void Emitter::update(float deltaTime, float currentTime, 
    std::shared_ptr<Shader> updateShader)
{
    // Update emitter position based on movement patterns.
    updateCircleMotion(currentTime);

    updateShader->use();

    // Set emitter uniforms for the update shader.
    updateShader->setFloat("deltaTime", deltaTime);
    updateShader->setVec2("emPos", m_position);
    updateShader->setVec2("emVelocityMin", m_velocityMin);
    updateShader->setVec2("emVelocityOffset", m_velocityOffset);
    updateShader->setVec2("emAcceleration", m_acceleration);
    updateShader->setFloat("emLifeMin", m_lifeMin);
    updateShader->setFloat("emLifeOffset", m_lifeOffset);
    updateShader->setFloat("emSize", m_size);

    // Set the seed.
    //std::random_device randdev;
    //std::mt19937 generator(randdev());
    std::mt19937 generator(static_cast<unsigned int>(currentTime));
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

    update();
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
    renderShader->setVec4("colour", m_colour);
    renderShader->setVec4("birthColour", m_birthColour);
    renderShader->setVec4("deathColour", m_deathColour);

    // Render the particles.
    render(renderShader);
}

void Emitter::clear(std::shared_ptr<Shader> updateShader)
{
    updateShader->use();
    updateShader->setBool("isClearParticles", true);

    update();

    updateShader->setBool("isClearParticles", false);
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
    m_origin = position;
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

void Emitter::setColour(glm::vec4 colour)
{
    m_colour = colour;
}

void Emitter::setLifeMin(float duration)
{
    m_lifeMin = duration;
}

void Emitter::setLifeOffset(float duration)
{
    m_lifeOffset = duration;
}

void Emitter::setBirthColour(glm::vec4 colour)
{
    m_birthColour = colour;
}

void Emitter::setDeathColour(glm::vec4 colour)
{
    m_deathColour = colour;
}

void Emitter::setCircleRadius(float radius)
{
    m_circleRadius = radius;
}

void Emitter::setCirclePeriod(float period)
{
    m_circlePeriod = period;
}

int Emitter::getNumToGenerate() const
{
	return m_numToGenerate;
}

glm::vec2 Emitter::getPosition() const
{
    return m_origin;
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

glm::vec4 Emitter::getColour() const
{
    return m_colour;
}

float Emitter::getLifeMin() const
{
    return m_lifeMin;
}

float Emitter::getLifeOffset() const
{
    return m_lifeOffset;
}

glm::vec4 Emitter::getBirthColour() const
{
    return m_birthColour;
}

glm::vec4 Emitter::getDeathColour() const
{
    return m_deathColour;
}

float Emitter::getCircleRadius() const
{
    return m_circleRadius;
}

float Emitter::getCirclePeriod() const
{
    return m_circlePeriod;
}

void Emitter::update()
{
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

    // Reset changes.
    glDisable(GL_RASTERIZER_DISCARD);
}

void Emitter::updateCircleMotion(float currentTime)
{
    // Avoid divide by zero.
    if (m_circlePeriod == 0)
    {
        m_position = m_origin;
        return;
    }

    // Parametric equation of circle: x = r*cos(t), y = r*sin(t)
    float time = currentTime / m_circlePeriod * glm::two_pi<float>();
    m_position.x = m_origin.x + m_circleRadius * glm::cos(time);
    m_position.y = m_origin.y + m_circleRadius * glm::sin(time);
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
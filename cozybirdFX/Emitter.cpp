#include "Emitter.h"
#include "AssetLoader.h"
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
    const int MS_PER_SECOND{ 1000 };
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

        // x/y position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)0);

        // Speed and direction
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)(2 * sizeof(float)));

        // Rotation, size, current particle life, and total particle life
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)(4 * sizeof(float)));

        // Type (emitter or particle)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(Particle), (void *)(8 * sizeof(float)));
    }
}

void Emitter::update(float deltaTime, float currentTime, 
    std::shared_ptr<Shader> updateShader)
{
    // Update emitter position based on movement patterns.
    updateMotion(currentTime);

    updateShader->use();

    // Set emitter uniforms for the update shader.
    updateShader->setFloat("deltaTime", deltaTime);
    updateShader->setVec2("emPos", m_position);
    updateShader->setVec3("emSpeed", m_speed);
    updateShader->setVec3("emDirection", glm::radians(glm::vec3(m_direction)));
    updateShader->setVec3("emRotation", glm::radians(glm::vec3(m_rotation)));
    updateShader->setVec2("emLife", m_life);
    updateShader->setVec3("emSize", m_size);

    // Set the seed.
    // Convert currentTime to ms to avoid precision loss when converting to
    // unsigned integer. Otherwise, the seed will be unchanged over 
    // multiple frames.
    std::mt19937 generator(static_cast<unsigned int>(currentTime * MS_PER_SECOND));
    std::uniform_real_distribution<> distrib(-100, 100);
    updateShader->setFloat("randomSeed", static_cast<float>(distrib(generator)));

    // Update spawn timer and flag the emitter to spawn particles if necessary.
    if (currentTime >= m_delayBeforeStart && 
        (currentTime < m_delayBeforeStart + m_emitterDuration || 
        m_emitterDuration == 0.f))
    {
        m_spawnTimer += deltaTime;
        if (m_spawnTimer >= m_timeToSpawn)
        {
            while (m_spawnTimer >= m_timeToSpawn)
                m_spawnTimer -= m_timeToSpawn;
            updateShader->setInt("emNumToGenerate", m_numToGenerate);
        }
        else
        {
            updateShader->setInt("emNumToGenerate", 0);
        }
    }
    else
    {
        // Get ready to spawn particles as soon as the next iteration starts.
        if (currentTime >= m_delayBeforeStart + m_emitterDuration)
            m_spawnTimer = m_timeToSpawn;

        updateShader->setInt("emNumToGenerate", 0);
    }

    update();
}

void Emitter::clear(std::shared_ptr<Shader> updateShader)
{
    updateShader->use();
    updateShader->setBool("isClearParticles", true);

    update();

    updateShader->setBool("isClearParticles", false);
    m_spawnTimer = m_timeToSpawn;
}

int Emitter::getNumParticles() const
{
    return m_numParticles;
}

void Emitter::setTexture(AssetLoader &assetLoader, const std::string &filePath)
{
    std::shared_ptr<Texture> texture{ assetLoader.load<Texture>(filePath) };
    if (texture != nullptr)
    {
        m_texture = texture;
        m_textureName = filePath;
    }
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

void Emitter::setSpeedMin(float speed)
{
    m_speed.x = speed;
}

void Emitter::setSpeedMax(float speed)
{
    m_speed.y = speed;
}

void Emitter::setSpeedGrowth(float amount)
{
    m_speed.z = amount;
}

void Emitter::setDirectionMin(int degAngle)
{
    m_direction.x = degAngle;
}

void Emitter::setDirectionMax(int degAngle)
{
    m_direction.y = degAngle;
}

void Emitter::setDirectionGrowth(int degAngle)
{
    m_direction.z = degAngle;
}

void Emitter::setRotationMin(int degAngle)
{
    m_rotation.x = degAngle;
}

void Emitter::setRotationMax(int degAngle)
{
    m_rotation.y = degAngle;
}

void Emitter::setRotationGrowth(int degAngle)
{
    m_rotation.z = degAngle;
}

void Emitter::setSizeMin(float size)
{
    m_size.x = size;
}

void Emitter::setSizeMax(float size)
{
    m_size.y = size;
}

void Emitter::setSizeGrowth(float amount)
{
    m_size.z = amount;
}

void Emitter::setLifeMin(float duration)
{
    m_life.x = duration;
}

void Emitter::setLifeMax(float duration)
{
    m_life.y = duration;
}

void Emitter::setColour(glm::vec4 colour)
{
    m_colour = colour;
}

void Emitter::setAdditivity(float additivity)
{
    m_additivity = additivity;
}

void Emitter::setBirthAdditivity(float additivity)
{
    m_birthAdditivity = additivity;
}

void Emitter::setDeathAdditivity(float additivity)
{
    m_deathAdditivity = additivity;
}

void Emitter::setBirthColour(glm::vec4 colour)
{
    m_birthColour = colour;
}

void Emitter::setDeathColour(glm::vec4 colour)
{
    m_deathColour = colour;
}

void Emitter::setDelayBeforeStart(float duration)
{
    m_delayBeforeStart = duration;
}

void Emitter::setEmitterDuration(float duration)
{
    m_emitterDuration = duration;
}

void Emitter::setHSineAmplitude(float amount)
{
    m_hSineAmplitude = amount;
}

void Emitter::setHSinePeriod(float period)
{
    m_hSinePeriod = period;
}

void Emitter::setVSineAmplitude(float amount)
{
    m_vSineAmplitude = amount;
}

void Emitter::setVSinePeriod(float period)
{
    m_vSinePeriod = period;
}

void Emitter::setCircleRadius(float radius)
{
    m_circleRadius = radius;
}

void Emitter::setCirclePeriod(float period)
{
    m_circlePeriod = period;
}

const std::string &Emitter::getTextureName() const
{
    return m_textureName;
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

glm::vec3 Emitter::getSpeed() const
{
    return m_speed;
}

glm::ivec3 Emitter::getDirection() const
{
    return m_direction;
}

glm::ivec3 Emitter::getRotation() const
{
    return m_rotation;
}

glm::vec3 Emitter::getSize() const
{
    return m_size;
}

glm::vec4 Emitter::getColour() const
{
    return m_colour;
}

glm::vec2 Emitter::getLife() const
{
    return m_life;
}

float Emitter::getAdditivity() const
{
    return m_additivity;
}

float Emitter::getBirthAdditivity() const
{
    return m_birthAdditivity;
}

float Emitter::getDeathAdditivity() const
{
    return m_deathAdditivity;
}

glm::vec4 Emitter::getBirthColour() const
{
    return m_birthColour;
}

glm::vec4 Emitter::getDeathColour() const
{
    return m_deathColour;
}

float Emitter::getDelayBeforeStart() const
{
    return m_delayBeforeStart;
}

float Emitter::getEmitterDuration() const
{
    return m_emitterDuration;
}

float Emitter::getHSineAmplitude() const
{
    return m_hSineAmplitude;
}

float Emitter::getHSinePeriod() const
{
    return m_hSinePeriod;
}

float Emitter::getVSineAmplitude() const
{
    return m_vSineAmplitude;
}

float Emitter::getVSinePeriod() const
{
    return m_vSinePeriod;
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

void Emitter::updateMotion(float currentTime)
{
    m_position = m_origin;

    // Oscillate horizontally. 
    if (m_hSinePeriod != 0)
    {
        float hTime = currentTime / m_hSinePeriod * glm::two_pi<float>();
        m_position.x += m_hSineAmplitude * glm::sin(hTime);
    }

    // Oscillate vertically.
    if (m_vSinePeriod != 0)
    {
        float vTime = currentTime / m_vSinePeriod * glm::two_pi<float>();
        m_position.y += m_vSineAmplitude * glm::sin(vTime);
    }

    // Parametric equation of circle: x = r*cos(t), y = r*sin(t)
    if (m_circlePeriod != 0)
    {
        float cTime = currentTime / m_circlePeriod * glm::two_pi<float>();
        m_position.x += m_circleRadius * glm::cos(cTime);
        m_position.y += m_circleRadius * glm::sin(cTime);
    }
}

void Emitter::render(std::shared_ptr<Shader> renderShader)
{
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // Enable rasterizer for graphical output.
    glDisable(GL_RASTERIZER_DISCARD);
    renderShader->use();

    renderShader->setVec4("colour", m_colour);
    renderShader->setVec4("birthColour", m_birthColour);
    renderShader->setVec4("deathColour", m_deathColour);
    renderShader->setVec3("additivity",
        glm::vec3(m_birthAdditivity, m_additivity, m_deathAdditivity));

    // Bind to the read buffer's vertex array object.
    glBindVertexArray(m_vao[m_currentReadBuffer]);

    // Disable velocity attribute since it isn't used for rendering.
    glDisableVertexAttribArray(1);

    // Draw the particles.
    m_texture->bind();
    glDrawArrays(GL_POINTS, 0, m_numParticles);

    // Reset configurations after rendering.
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}
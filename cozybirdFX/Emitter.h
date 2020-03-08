#pragma once

#include <glm/glm.hpp>

#include <memory>

class Camera;
class Shader;
class Texture;

class Emitter
{
public:
	Emitter();

	void update(float deltaTime, float currentTime, std::shared_ptr<Shader> updateShader);

	// Render to scene.
	void render(Camera *camera, std::shared_ptr<Shader> renderShader);

	// Render to frame buffer.
	void render(std::shared_ptr<Shader> renderShader);

	void clear();

	int getNumParticles() const;

	// Setter functions.
	void setTexture(std::shared_ptr<Texture> texture);
	void setNumToGenerate(int num);
	void setPosition(glm::vec2 position);
	void setTimeToSpawn(float duration);
	void setVelocityMin(glm::vec2 velocity);
	void setVelocityOffset(glm::vec2 velocity);
	void setAcceleration(glm::vec2 acceleration);
	void setSize(float size);
	void setColour(glm::vec4 colour);
	void setLifeMin(float duration);
	void setLifeOffset(float duration);
	void setBirthColour(glm::vec4 colour);
	void setDeathColour(glm::vec4 colour);

	// Getter functions.
	int getNumToGenerate() const;
	glm::vec2 getPosition() const;
	float getTimeToSpawn() const;
	glm::vec2 getVelocityMin() const;
	glm::vec2 getVelocityOffset() const;
	glm::vec2 getAcceleration() const;
	float getSize() const;
	glm::vec4 getColour() const;
	float getLifeMin() const;
	float getLifeOffset() const;
	glm::vec4 getBirthColour() const;
	glm::vec4 getDeathColour() const;

private:
	enum ParticleType
	{
		TYPE_EMITTER = 0,
		TYPE_PARTICLE = 1
	};

	struct Particle
	{
		glm::vec2 position;
		glm::vec2 velocity;
		float currentLife;
		float life;
		float size;

		// This is the integer representation of ParticleType.
		int type;
	};

	// Transform feedback buffer.
	// This is used to hold output values from the geometry shader.
	unsigned int m_transFeedbackBuffer{ 0 };

	// Query object.
	// This is used to check how many particles have been emitted.
	unsigned int m_query{ 0 };

	// Use double buffers to hold particle data.
	// Alternate between each, one used as a read buffer and the other as a write
	// buffer.
	unsigned int m_particleBuffers[2];
	unsigned int m_vao[2];
	int m_currentReadBuffer{ 0 };

	// The particle's texture.
	std::shared_ptr<Texture> m_texture;

	// The current number of particles in the system.
	// Initialize with one emitter.
	int m_numParticles{ 1 };

	// The number of particles to generate.
	int m_numToGenerate{ 30 };

	// The origin of the emitter.
	glm::vec2 m_position{ 0.f };

	// The time before spawning a new particle.
	float m_timeToSpawn{ 0.5f };

	// The time since the last particle generation.
	float m_spawnTimer{ 0.f };

	// The velocity at which the particle moves at.
	// Defines the minimum velocity and the offset from that minimum.
	glm::vec2 m_velocityMin{ -32.f };
	glm::vec2 m_velocityOffset{ 64.f };

	// Acceleration vector.
	glm::vec2 m_acceleration{ 0.f };

	// The size of the particle.
	float m_size{ 32.f };

	// The colour of the particle.
	glm::vec4 m_colour{ 0.2f, 0.2f, 0.2f, 1.f };

	// The time to live for the particle.
	float m_lifeMin{ 2.f };
	float m_lifeOffset{ 0.3f };

	// The colour of the particle when it is created.
	glm::vec4 m_birthColour{ 0.2f, 0.2f, 0.2f, 0.f };

	// The colour of the particle when it is expiring.
	glm::vec4 m_deathColour{ 0.2f, 0.2f, 0.2f, 0.f };
};
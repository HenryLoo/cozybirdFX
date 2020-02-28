#pragma once

#include <glm/glm.hpp>

#include <memory>

class AssetLoader;
class Camera;
class Shader;
class Texture;

class Emitter
{
public:
	Emitter(AssetLoader *assetLoader);

	void update(float deltaTime);
	void render(Camera *camera);

	void clear();

	int getNumParticles() const;

	// Property setters.
	void setTexture(std::shared_ptr<Texture> texture);
	void setNumToGenerate(int num);
	void setPosition(glm::vec2 position);
	void setTimeToSpawn(float duration);
	void setVelocityMin(glm::vec2 velocity);
	void setVelocityOffset(glm::vec2 velocity);
	void setAcceleration(glm::vec2 acceleration);
	void setSize(float size);
	void setColour(glm::vec3 colour);
	void setDurationMin(float duration);
	void setDurationOffset(float duration);

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
		glm::vec3 colour;
		float duration;
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
	int m_numToGenerate{ 0 };

	// The origin of the emitter.
	glm::vec2 m_position{ 0.f };

	// The time before spawning a new particle.
	float m_timeToSpawn{ 0.5f };

	// The time since the last particle generation.
	float m_spawnTimer{ 0.f };

	// The velocity at which the particle moves at.
	// Defines the minimum velocity and the offset from that minimum.
	glm::vec2 m_velocityMin{ 0.f };
	glm::vec2 m_velocityOffset{ 0.f };

	// Acceleration vector.
	glm::vec2 m_acceleration{ 0.f };

	// The size of the particle.
	float m_size{ 1.f };

	// The colour of the particle.
	glm::vec3 m_colour{ 1.f };

	// The time to live for the particle.
	float m_durationMin{ 1.f };
	float m_durationOffset{ 0.5f };

	std::shared_ptr<Shader> m_updateShader{ nullptr };
	std::shared_ptr<Shader> m_renderShader{ nullptr };
};
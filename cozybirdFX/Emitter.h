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

	void update(float deltaTime, std::shared_ptr<Shader> updateShader);
	void render(Camera *camera, std::shared_ptr<Shader> renderShader);

	void clear();

	int getNumParticles() const;

	Texture *getOutputTexture() const;

	// Setter functions.
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

	// Getter functions.
	int getNumToGenerate() const;
	glm::vec2 getPosition() const;
	float getTimeToSpawn() const;
	glm::vec2 getVelocityMin() const;
	glm::vec2 getVelocityOffset() const;
	glm::vec2 getAcceleration() const;
	float getSize() const;
	glm::vec3 getColour() const;
	float getDurationMin() const;
	float getDurationOffset() const;

	void outputToTexture(std::shared_ptr<Shader> updateShader,
		std::shared_ptr<Shader> renderShader);

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

	void render(std::shared_ptr<Shader> renderShader);

	void createFramebuffer(glm::ivec2 textureSize);

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
	glm::vec3 m_colour{ 0.2f };

	// The time to live for the particle.
	float m_durationMin{ 2.f };
	float m_durationOffset{ 0.3f };

	// The duration of the emitter's animation.
	float m_currentTime{ 0.f };
	float m_emitterDuration{ 3.f };

	// Buffers for rendering to texture.
	unsigned int m_fbo{ 0 };
	std::shared_ptr<Texture> m_outputTexture{ nullptr };

	glm::ivec2 m_clipSize{ 400.f, 400.f };
};
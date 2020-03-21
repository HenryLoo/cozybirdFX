#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <string>

class AssetLoader;
class Camera;
class Shader;
class Texture;

class Emitter
{
public:
	Emitter();

	void update(float deltaTime, float currentTime, std::shared_ptr<Shader> updateShader);

	// Render to scene.
	void render(const Camera &camera, std::shared_ptr<Shader> renderShader);

	// Render to frame buffer.
	void render(std::shared_ptr<Shader> renderShader);

	// Clear all particles for this emitter.
	void clear(std::shared_ptr<Shader> updateShader);

	int getNumParticles() const;

	// Setter functions.
	void setTexture(AssetLoader &assetLoader, const std::string &filePath);
	void setNumToGenerate(int num);
	void setPosition(glm::vec2 position);
	void setTimeToSpawn(float duration);
	void setVelocityMin(glm::vec2 velocity);
	void setVelocityOffset(glm::vec2 velocity);
	void setAcceleration(glm::vec2 acceleration);
	void setSize(float size);
	void setColour(glm::vec4 colour);
	void setAdditivity(float additivity);
	void setBirthAdditivity(float additivity);
	void setDeathAdditivity(float additivity);
	void setLifeMin(float duration);
	void setLifeOffset(float duration);
	void setBirthColour(glm::vec4 colour);
	void setDeathColour(glm::vec4 colour);
	void setDelayBeforeStart(float duration);
	void setEmitterDuration(float duration);
	void setHSineAmplitude(float amount);
	void setHSinePeriod(float period);
	void setVSineAmplitude(float amount);
	void setVSinePeriod(float period);
	void setCircleRadius(float radius);
	void setCirclePeriod(float period);

	// Getter functions.
	const std::string &getTextureName() const;
	int getNumToGenerate() const;
	glm::vec2 getPosition() const;
	float getTimeToSpawn() const;
	glm::vec2 getVelocityMin() const;
	glm::vec2 getVelocityOffset() const;
	glm::vec2 getAcceleration() const;
	float getSize() const;
	glm::vec4 getColour() const;
	float getAdditivity() const;
	float getBirthAdditivity() const;
	float getDeathAdditivity() const;
	float getLifeMin() const;
	float getLifeOffset() const;
	glm::vec4 getBirthColour() const;
	glm::vec4 getDeathColour() const;
	float getDelayBeforeStart() const;
	float getEmitterDuration() const;
	float getHSineAmplitude() const;
	float getHSinePeriod() const;
	float getVSineAmplitude() const;
	float getVSinePeriod() const;
	float getCircleRadius() const;
	float getCirclePeriod() const;

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

	// Common update code used by both regular updates and particle clears.
	void update();

	// Move the emitter according to its sine and circle motion parameters.
	void updateMotion(float currentTime);

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
	std::string m_textureName;

	// The current number of particles in the system.
	// Initialize with one emitter.
	int m_numParticles{ 1 };

	// The number of particles to generate.
	int m_numToGenerate{ 20 };

	// The origin position of the emitter.
	glm::vec2 m_origin{ 0.f };

	// The final position of the emitter, after performing translations.
	glm::vec2 m_position{ 0.f };

	// The time before spawning a new particle.
	float m_timeToSpawn{ 0.5f };

	// The time since the last particle generation.
	float m_spawnTimer{ 0.f };

	// The velocity at which the particle moves at.
	// Defines the minimum velocity and the offset from that minimum.
	glm::vec2 m_velocityMin{ -48.f };
	glm::vec2 m_velocityOffset{ 96.f };

	// Acceleration vector.
	glm::vec2 m_acceleration{ 0.f };

	// The size of the particle.
	float m_size{ 16.f };

	// The colour of the particle.
	glm::vec4 m_colour{ 0.2f, 0.2f, 1.f, 1.f };
	float m_additivity{ 0.f };

	// The time to live for the particle.
	float m_lifeMin{ 2.f };
	float m_lifeOffset{ 0.3f };

	// The colour of the particle when it is created.
	glm::vec4 m_birthColour{ 0.2f, 0.2f, 1.f, 0.f };
	float m_birthAdditivity{ 0.f };

	// The colour of the particle when it is expiring.
	glm::vec4 m_deathColour{ 0.2f, 0.2f, 1.f, 0.f };
	float m_deathAdditivity{ 0.f };

	// The time from animation start before starting to emit particles.
	float m_delayBeforeStart{ 0.f };

	// The time before stopping particle emission.
	float m_emitterDuration{ 0.f };

	// Defines the sine movement pattern for this emitter.
	float m_hSineAmplitude{ 0.f };
	float m_hSinePeriod{ 0.f };
	float m_vSineAmplitude{ 0.f };
	float m_vSinePeriod{ 0.f };

	// Defines the circle movement pattern for this emitter.
	float m_circleRadius{ 0.f };
	float m_circlePeriod{ 0.f };
};
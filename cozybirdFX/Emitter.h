#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <string>

class AssetLoader;
class Camera;
class Shader;
class Texture;

class Emitter
{
public:
	enum class BlendMode
	{
		Linear,
		Additive,
	};

	Emitter();

	void update(float deltaTime, float currentTime, 
		std::shared_ptr<Shader> updateShader, bool isLooping);

	// Render to the active frame buffer.
	void render(std::shared_ptr<Shader> renderShader);

	// Clear all particles for this emitter.
	void clear(std::shared_ptr<Shader> updateShader);

	int getNumParticles() const;

	// Setter functions.
	void setTexture(AssetLoader &assetLoader, const std::string &filePath);
	void setNumToGenerate(int num);
	void setPosition(glm::vec2 position);
	void setDistribution(glm::vec2 widthHeight);
	void setTimeToSpawn(float duration);
	void setSpeedMin(float speed);
	void setSpeedMax(float speed);
	void setSpeedGrowth(float amount);
	void setDirectionMin(int degAngle);
	void setDirectionMax(int degAngle);
	void setDirectionGrowth(int degAngle);
	void setRotationMin(int degAngle);
	void setRotationMax(int degAngle);
	void setRotationGrowth(int degAngle);
	void setSizeMin(float size);
	void setSizeMax(float size);
	void setSizeGrowth(float amount);
	void setLifeMin(float duration);
	void setLifeMax(float duration);
	void setBlendMode(BlendMode mode);
	void setColour(glm::vec4 colour);
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
	glm::vec2 getDistribution() const;
	float getTimeToSpawn() const;
	glm::vec3 getSpeed() const;
	glm::ivec3 getDirection() const;
	glm::ivec3 getRotation() const;
	glm::vec3 getSize() const;
	glm::vec2 getLife() const;
	BlendMode getBlendMode() const;
	glm::vec4 getColour() const;
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

		// Speed, direction of movement.
		glm::vec2 speedDirection;

		// Rotation, size, current life, total life.
		glm::vec4 rotationSizeLife;

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

	// The width and height of the box in which particles are randomly spawned 
	// in. A distribution of width and height 0 means that particles will
	// always spawn at the origin point.
	glm::vec2 m_distribution{ 0.f };

	// The time before spawning a new particle.
	float m_timeToSpawn{ 0.5f };

	// The time since the last particle generation.
	float m_spawnTimer{ 0.f };

	// The speed at which the particle moves at.
	// Defines minimum, maximum, and growth rate.
	glm::vec3 m_speed{ 0.f, 48.f, 0.f };

	// The direction that the particle moves toward.
	// Defines minimum, maximum, and growth rate.
	glm::ivec3 m_direction{ 0, 359, 0 };

	// The rotation of the particle.
	// Defines minimum, maximum, and growth rate.
	glm::ivec3 m_rotation{ 0, 0, 0 };

	// The size of the particle.
	// Defines minimum, maximum, and growth rate.
	glm::vec3 m_size{ 16.f, 16.f, 0.f };

	// The time to live for the particle.
	// Defines minimum and maximum.
	glm::vec2 m_life{ 2.f, 2.3f };

	// The blend mode of particles.
	BlendMode m_blendMode{ BlendMode::Linear };

	// The colour of the particle.
	glm::vec4 m_colour{ 0.2f, 0.2f, 1.f, 1.f };

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
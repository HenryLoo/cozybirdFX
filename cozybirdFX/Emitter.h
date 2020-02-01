#pragma once

#include <glm/glm.hpp>

struct Emitter
{
	// The origin of the emitter.
	glm::vec3 position{ 0.f };

	// The rate of particle generation.
	float spawnRate{ 0.f };

	// The current time left before spawning another particle.
	float spawnTimer{ 0.f };

	// The speed at which the particle moves at.
	float speed{ 0.f };

	// The size of the particle.
	glm::vec3 size{ 1.f };

	// The colour of the particle.
	glm::vec4 colour{ 1.f };

	// The time to live for the particle.
	float duration{ 0.f };
};
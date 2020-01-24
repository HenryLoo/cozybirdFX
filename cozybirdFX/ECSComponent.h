#pragma once

#include <glm/glm.hpp>

namespace ECSComponent
{
	// Defines the different component bitmasks, in powers of 2.
	enum Type
	{
		NONE = 0,
		POSITION = 1,
		VELOCITY = 2,
		TRANSFORM = 4,
		SPRITE = 8,
	};

	struct Position
	{
		glm::vec3 xyz;
	};

	struct Velocity
	{
		glm::vec3 xyz;
	};

	struct Transform
	{
		// Size multiplier in xyz axes.
		glm::vec3 scale{ 1.f };

		// Rotation about xyz axes, in radians.
		glm::vec3 rotation{ 0.f };
	};

	struct Sprite
	{
		unsigned int textureId;

		// Blended colour.
		glm::vec4 rgba{ 1.f };
	};
}
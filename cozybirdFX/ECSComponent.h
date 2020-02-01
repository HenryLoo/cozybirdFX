#pragma once

#include <glm/glm.hpp>

namespace ECSComponent
{
	// Defines the different component bitmasks, in powers of 2.
	enum Type
	{
		COMPONENT_NONE = 0,
		COMPONENT_POSITION = 1,
		COMPONENT_VELOCITY = 2,
		COMPONENT_TRANSFORM = 4,
		COMPONENT_SPRITE = 8,
		COMPONENT_DECAY = 16,
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

	struct Decay
	{
		// Duration of time in seconds before this entity should be deleted.
		float duration;
	};
}
#include "PhysicsSystem.h"

namespace
{
	const unsigned long COMPONENTS_MASK
	{
		ECSComponent::COMPONENT_POSITION |
		ECSComponent::COMPONENT_VELOCITY
	};
}


PhysicsSystem::PhysicsSystem(EntityManager &manager, 
	std::vector<ECSComponent::Position> &positions, 
	std::vector<ECSComponent::Velocity> &velocities) :
	ECSSystem(manager, COMPONENTS_MASK), m_positions(positions),
	m_velocities(velocities)
{
}

void PhysicsSystem::updateEntity(int entityId, float deltaTime)
{
	m_positions[entityId].xyz += m_velocities[entityId].xyz * deltaTime;
}

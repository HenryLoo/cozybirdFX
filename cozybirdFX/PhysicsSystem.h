#pragma once

#include "ECSSystem.h"
#include "ECSComponent.h"

#include <vector>

class PhysicsSystem : public ECSSystem
{
public:
	PhysicsSystem(EntityManager &manager,
		std::vector<ECSComponent::Position> &positions,
		std::vector<ECSComponent::Velocity> &velocities);

protected:
	virtual void updateEntity(int entityId, float deltaTime) override;

private:
	// Component references.
	std::vector<ECSComponent::Position> &m_positions;
	std::vector<ECSComponent::Velocity> &m_velocities;
};
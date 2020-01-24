#pragma once

#include "ECSComponent.h"

#include <vector>

class ECSSystem;

class EntityManager
{
public:
	// Call the update function for each ECSSystem to update all entities.
	void update(float deltaTime);

	// Create a new entity and return its id.
	int createEntity();

	// Delete an existing entity, given its id.
	void deleteEntity(int id);

private:
	static const int MAX_ENTITIES{ 10000 };

	// Hold bitmasks for all entities to specify which components each entity has.
	// The index of this vector corresponds to the entity'd id.
	std::vector<unsigned long> m_entities;

	// The current number of entities.
	int m_numEntities{ 0 };

	// Hold all ECSSystems.
	std::vector<ECSSystem> m_systems;

	// Hold all components.
	// The indices of these vectors correspond to the entity'd id.
	std::vector<ECSComponent::Position> m_componentPositions;
	std::vector<ECSComponent::Velocity> m_componentVelocities;
	std::vector<ECSComponent::Transform> m_componentTranforms;
	std::vector<ECSComponent::Sprite> m_componentSprites;
};
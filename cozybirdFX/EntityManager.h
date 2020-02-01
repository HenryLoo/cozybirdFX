#pragma once

#include "ECSComponent.h"
#include "ECSSystem.h"

#include <memory>
#include <vector>

struct GLFWwindow;

class SpriteRenderer;

class EntityManager
{
public:
	EntityManager(SpriteRenderer *sRenderer);

	// Call the update function for each ECSSystem to update all entities.
	void update(float deltaTime);

	// Create a new entity and return its id.
	int createEntity();

	// Delete an existing entity, given its id.
	void deleteEntity(int entityId);

	// Get the number of entities.
	int getNumEntities() const;

	void addComponent(int entityId, ECSComponent::Type typeMask);
	void removeComponent(int entityId, ECSComponent::Type typeMask);

private:
	static const int MAX_ENTITIES{ 10000 };

	// Register a new ECSSystem.
	// This should be called as part of instantiation.
	void addSystem(ECSSystem *system);

	inline bool isValidEntity(int entityId) const;

	// Hold bitmasks for all entities to specify which components each entity has.
	// The index of this vector corresponds to the entity'd id.
	std::vector<unsigned long> m_entities;

	// The current number of entities.
	int m_numEntities{ 0 };

	// Hold all ECSSystems.
	std::vector<std::unique_ptr<ECSSystem>> m_systems;

	// Hold all components.
	// The indices of these vectors correspond to the entity'd id.
	std::vector<ECSComponent::Position> m_componentPositions;
	std::vector<ECSComponent::Velocity> m_componentVelocities;
	std::vector<ECSComponent::Transform> m_componentTransforms;
	std::vector<ECSComponent::Sprite> m_componentSprites;
	std::vector<ECSComponent::Decay> m_componentDecays;
};
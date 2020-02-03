#include "EntityManager.h"
#include "EmitterSystem.h"
#include "PhysicsSystem.h"
#include "SpriteSystem.h"

EntityManager::EntityManager(SpriteRenderer *sRenderer, AssetLoader *assetLoader)
{
	m_entities.resize(MAX_ENTITIES);
	m_componentPositions.resize(MAX_ENTITIES);
	m_componentVelocities.resize(MAX_ENTITIES);
	m_componentTransforms.resize(MAX_ENTITIES);
	m_componentSprites.resize(MAX_ENTITIES);
	m_componentDecays.resize(MAX_ENTITIES);

	// Initialize ECSSystems.
	addSystem(new EmitterSystem(*this, assetLoader, m_componentSprites,
		m_componentPositions, m_componentVelocities,
		m_componentTransforms, m_componentDecays));
	addSystem(new PhysicsSystem(*this, m_componentPositions, 
		m_componentVelocities));
	addSystem(new SpriteSystem(*this, sRenderer, m_componentSprites, 
		m_componentPositions, m_componentTransforms));
}

void EntityManager::update(float deltaTime)
{
	for (auto &system : m_systems)
	{
		system->update(deltaTime);
	}
}

int EntityManager::createEntity()
{
	// Initialize the new entity.
	int entityId{ m_numEntities };
	m_entities[entityId] = ECSComponent::COMPONENT_NONE;

	// Reset the components.
	m_componentPositions[entityId] = {};
	m_componentVelocities[entityId] = {};
	m_componentTransforms[entityId] = {};
	m_componentSprites[entityId] = {};
	m_componentDecays[entityId] = {};

	++m_numEntities;

	return entityId;
}

void EntityManager::deleteEntity(int entityId)
{
	if (!isValidEntity(entityId))
		return;

	// Reset the bitmask for the entity to delete.
	m_entities[entityId] = ECSComponent::COMPONENT_NONE;

	// Swap this entity's position with the entity at the back of the array
	// to maintain contiguity.
	int lastId{ m_numEntities - 1 };
	std::swap(m_entities[entityId], m_entities[lastId]);

	// Swap entity components too.
	std::swap(m_componentPositions[entityId], m_componentPositions[lastId]);
	std::swap(m_componentVelocities[entityId], m_componentVelocities[lastId]);
	std::swap(m_componentTransforms[entityId], m_componentTransforms[lastId]);
	std::swap(m_componentSprites[entityId], m_componentSprites[lastId]);
	std::swap(m_componentDecays[entityId], m_componentDecays[lastId]);

	--m_numEntities;
}

int EntityManager::getNumEntities() const
{
	return m_numEntities;
}

void EntityManager::addComponent(int entityId, ECSComponent::Type typeMask)
{
	if (!isValidEntity(entityId) && m_numEntities < MAX_ENTITIES)
		return;

	m_entities[entityId] |= typeMask;
}

void EntityManager::removeComponent(int entityId, ECSComponent::Type typeMask)
{
	if (!isValidEntity(entityId))
		return;

	// Clear the bit in the component mask.
	m_entities[entityId] &= ~(1UL << typeMask);
}

void EntityManager::addSystem(ECSSystem *system)
{
	m_systems.push_back(std::move(std::unique_ptr<ECSSystem>(system)));
}

inline bool EntityManager::isValidEntity(int entityId) const
{
	return entityId >= 0 && entityId < m_numEntities;
}

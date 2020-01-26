#include "EntityManager.h"
#include "SpriteSystem.h"

EntityManager::EntityManager(SpriteRenderer *sRenderer)
{
	m_entities.resize(MAX_ENTITIES);
	m_componentPositions.resize(MAX_ENTITIES);
	m_componentVelocities.resize(MAX_ENTITIES);
	m_componentTransforms.resize(MAX_ENTITIES);
	m_componentSprites.resize(MAX_ENTITIES);

	// Initialize ECSSystems.
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

	++m_numEntities;

	return entityId;
}

void EntityManager::deleteEntity(int id)
{
}

int EntityManager::getNumEntities() const
{
	return m_numEntities;
}

void EntityManager::addComponent(int entityId, ECSComponent::Type typeMask)
{
	if (!isValidEntity(entityId))
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

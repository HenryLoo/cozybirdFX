#include "ECSSystem.h"
#include "EntityManager.h"

ECSSystem::ECSSystem(EntityManager &entityManager, unsigned long componentsMask) :
	m_entityManager(entityManager), m_componentsMask(componentsMask)
{
}

void ECSSystem::update(float deltaTime)
{
	updateSystem(deltaTime);

	for (int i = 0; i < m_entityManager.getNumEntities(); ++i)
	{
		updateEntity(i, deltaTime);
	}
}

void ECSSystem::updateSystem(float deltaTime)
{
	// Do nothing by default.
}

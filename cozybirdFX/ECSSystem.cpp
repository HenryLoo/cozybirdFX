#include "ECSSystem.h"
#include "EntityManager.h"

ECSSystem::ECSSystem(EntityManager &manager, unsigned long componentsMask) :
	m_manager(manager), m_componentsMask(componentsMask)
{
}

void ECSSystem::update(float deltaTime)
{
	for (int i = 0; i < m_manager.getNumEntities(); ++i)
	{
		updateEntity(i, deltaTime);
	}
}

#include "ECSSystem.h"

ECSSystem::ECSSystem(EntityManager &manager, unsigned long componentsMask) :
	m_manager(manager), m_componentsMask(componentsMask)
{
}

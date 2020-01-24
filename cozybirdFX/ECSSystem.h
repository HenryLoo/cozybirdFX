#pragma once

class EntityManager;

class ECSSystem
{
public:
	ECSSystem(EntityManager &manager, unsigned long componentsMask);

	// Update all entities and their components.
	virtual void update(float deltaTime) = 0;

protected:
	// Hold a reference to the entity manager.
	EntityManager &m_manager;

	// This determines the components that this system will use.
	unsigned long m_componentsMask;
};
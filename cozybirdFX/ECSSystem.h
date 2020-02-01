#pragma once

class EntityManager;

class ECSSystem
{
public:
	ECSSystem(EntityManager &entityManager, unsigned long componentsMask);

	// Update all entities and their components.
	void update(float deltaTime);

protected:
	// Implement specific per-system updates. This is optional.
	virtual void updateSystem(float deltaTime);

	// Implement specific per-entity updates.
	virtual void updateEntity(int entityId, float deltaTime) = 0;

	// Hold a reference to the entity manager.
	EntityManager &m_entityManager;

	// This determines the components that this system will use.
	unsigned long m_componentsMask;
};
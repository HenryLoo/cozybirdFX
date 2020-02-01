#include "EmitterSystem.h"
#include "EntityManager.h"

#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

namespace
{
	const unsigned long COMPONENTS_MASK
	{
		ECSComponent::COMPONENT_SPRITE |
		ECSComponent::COMPONENT_POSITION |
        ECSComponent::COMPONENT_VELOCITY |
		ECSComponent::COMPONENT_TRANSFORM |
        ECSComponent::COMPONENT_DECAY
	};

    const int DEGREES_PER_RADIAN{ 360 };
}

EmitterSystem::EmitterSystem(EntityManager &manager,
	std::vector<ECSComponent::Sprite> &sprites, 
	std::vector<ECSComponent::Position> &positions,
	std::vector<ECSComponent::Velocity> &velocities,
	std::vector<ECSComponent::Transform> &transforms,
    std::vector<ECSComponent::Decay> &decays) :
	ECSSystem(manager, COMPONENTS_MASK), m_sprites(sprites), m_positions(positions), 
    m_velocities(velocities), m_transforms(transforms), m_decays(decays)
{
    // TODO: Initialize with a test emitter. Remove this later.
    Emitter em;
    em.spawnRate = 0.25f;
    em.speed = 0.5f;
    em.duration = 3.f;
    m_emitters.push_back(em);
}

void EmitterSystem::updateSystem(float deltaTime)
{
    // Update each emitter.
    for (Emitter &em : m_emitters)
    {
        em.spawnTimer -= deltaTime;

        // Create particle if timer is over.
        if (em.spawnTimer <= 0.f)
        {
            // Reset the timer.
            em.spawnTimer = em.spawnRate;

            // Create the entity and its components.
            int id{ m_entityManager.createEntity() };
            m_entityManager.addComponent(id, ECSComponent::COMPONENT_POSITION);
            m_entityManager.addComponent(id, ECSComponent::COMPONENT_SPRITE);
            m_entityManager.addComponent(id, ECSComponent::COMPONENT_VELOCITY);
            m_entityManager.addComponent(id, ECSComponent::COMPONENT_TRANSFORM);
            m_entityManager.addComponent(id, ECSComponent::COMPONENT_DECAY);

            // Set component values.
            m_positions[id].xyz = em.position;
            m_sprites[id].rgba = em.colour;

            glm::vec3 vel{ 0.f, em.speed, 0.f };
            m_velocities[id].xyz = glm::rotateZ(vel, 
                glm::radians(rand() % DEGREES_PER_RADIAN * 1.f));

            m_transforms[id].rotation = glm::vec3(0.f, 0.f, 
                glm::radians(rand() % DEGREES_PER_RADIAN * 1.f));
            m_transforms[id].scale = em.size;
            m_decays[id].duration = em.duration;
        }
    }
}

void EmitterSystem::updateEntity(int entityId, float deltaTime)
{
    // Update decay durations and delete the entity if its duration is over.
    m_decays[entityId].duration -= deltaTime;
    if (m_decays[entityId].duration <= 0)
    {
        m_entityManager.deleteEntity(entityId);
    }
}

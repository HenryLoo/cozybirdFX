#pragma once

#include "ECSSystem.h"
#include "ECSComponent.h"
#include "Emitter.h"

#include <list>
#include <vector>

struct GLFWwindow;

class AssetLoader;

class EmitterSystem : public ECSSystem
{
public:
	EmitterSystem(EntityManager &manager,
		AssetLoader *assetLoader,
		std::vector<ECSComponent::Sprite> &sprites,
		std::vector<ECSComponent::Position> &positions,
		std::vector<ECSComponent::Velocity> &velocities,
		std::vector<ECSComponent::Transform> &transforms,
		std::vector<ECSComponent::Decay> &decays);

protected:
	virtual void updateSystem(float deltaTime) override;
	virtual void updateEntity(int entityId, float deltaTime) override;

private:
	// Hold all emitters in the system.
	std::list<Emitter> m_emitters;

	// Component references.
	std::vector<ECSComponent::Sprite> &m_sprites;
	std::vector<ECSComponent::Position> &m_positions;
	std::vector<ECSComponent::Velocity> &m_velocities;
	std::vector<ECSComponent::Transform> &m_transforms;
	std::vector<ECSComponent::Decay> &m_decays;
};
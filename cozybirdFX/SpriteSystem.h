#pragma once

#include "ECSSystem.h"
#include "ECSComponent.h"

#include <vector>

class SpriteRenderer;

class SpriteSystem : public ECSSystem
{
public:
	SpriteSystem(EntityManager &manager,
		SpriteRenderer *sRenderer,
		std::vector<ECSComponent::Sprite> &sprites,
		std::vector<ECSComponent::Position> &positions,
		std::vector<ECSComponent::Transform> &transforms);

protected:
	virtual void updateEntity(int entityId, float deltaTime) override;

private:
	// Pointer to the EntityManager's renderer.
	SpriteRenderer *m_sRenderer{ nullptr };

	// Component references.
	std::vector<ECSComponent::Sprite> &m_sprites;
	std::vector<ECSComponent::Position> &m_positions;
	std::vector<ECSComponent::Transform> &m_transforms;
};
#include "SpriteSystem.h"
#include "SpriteRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
	const unsigned long COMPONENTS_MASK
	{ 
		ECSComponent::COMPONENT_SPRITE |
		ECSComponent::COMPONENT_POSITION |
		ECSComponent::COMPONENT_TRANSFORM 
	};
}

SpriteSystem::SpriteSystem(EntityManager &manager,
	SpriteRenderer *sRenderer,
	std::vector<ECSComponent::Sprite> &sprites,
	std::vector<ECSComponent::Position> &positions,
	std::vector<ECSComponent::Transform> &transforms) :
	ECSSystem(manager, COMPONENTS_MASK), m_sRenderer(sRenderer),
	m_sprites(sprites), m_positions(positions), m_transforms(transforms)
{
}

void SpriteSystem::updateEntity(int entityId, float deltaTime)
{
	glm::mat4 model{ 1.f };
	model = glm::translate(model, m_positions[entityId].xyz);
	model = glm::scale(model, m_transforms[entityId].scale);
	model = glm::rotate(model, m_transforms[entityId].rotation.x, glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, m_transforms[entityId].rotation.y, glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, m_transforms[entityId].rotation.z, glm::vec3(0.f, 0.f, 1.f));

	m_sRenderer->addSprite(model);

	// TODO: Test rotation update. Remove this later.
	m_transforms[entityId].rotation.y += glm::radians(90.0f) * deltaTime;
}

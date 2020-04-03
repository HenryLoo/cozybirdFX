#include "UISprite.h"
#include <iostream>
UISprite::UISprite(glm::vec2 size, glm::vec2 position) :
	IUserInterface(position, size, glm::vec4(0.f, 0.f, 0.f, 0.5f), true)
{
}

void UISprite::addToRenderer(SpriteRenderer &sRenderer, UIRenderer &uRenderer, 
	TextRenderer &tRenderer)
{
	IUserInterface::addToRenderer(sRenderer, uRenderer, tRenderer);

	glm::vec2 spritePos{ m_position + m_offset };

	// Add the label to the renderer.
	SpriteRenderer::Properties prop;
	prop.pos = spritePos + m_size / 2.f;
	prop.size = m_size;
	auto it{ sRenderer.addSprite(prop) };
	m_sProperties = &*it;
}

void UISprite::setPosition(glm::vec2 position)
{
	glm::vec2 oldPos{ m_position };
	IUserInterface::setPosition(position);

	glm::vec2 diff{ m_position - oldPos };
	if (m_sProperties != nullptr)
		m_sProperties->pos += diff;
}

void UISprite::setEnabled(bool isEnabled)
{
	IUserInterface::setEnabled(isEnabled);

	if (m_sProperties != nullptr)
		m_sProperties->isEnabled = isEnabled;
}

void UISprite::setSprite(std::shared_ptr<Texture> texture)
{
	if (texture == nullptr)
		return;

	m_texture = texture;
	if (m_sProperties != nullptr)
		m_sProperties->texture = m_texture.get();
}

bool UISprite::handleInput(InputManager &inputManager, UndoableAction &action)
{
	return false;
}

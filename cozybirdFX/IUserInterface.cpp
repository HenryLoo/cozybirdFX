#include "IUserInterface.h"
#include "InputManager.h"

bool IUserInterface::m_isAnyClicked{ false };

IUserInterface::IUserInterface(glm::vec2 position, glm::vec2 size,
	glm::vec4 colour, bool hasBorder) : m_position(position),
	m_size(size), m_colour(colour), m_hasBorder(hasBorder)
{
}

bool IUserInterface::process(InputManager &inputManager, 
	UndoableAction &action)
{
	// Skip disabled elements.
	if (!m_isEnabled)
		return false;

	return handleInput(inputManager, action);
}

void IUserInterface::addToRenderer(SpriteRenderer &sRenderer, 
	UIRenderer &uRenderer, TextRenderer &tRenderer)
{
	UIRenderer::Properties prop;
	prop.pos = m_position + m_offset;
	prop.size = m_size;
	prop.colour = m_colour;
	prop.hasBorder = m_hasBorder;
	auto it{ uRenderer.addElement(prop) };
	m_uiProperties = &*it;
}

void IUserInterface::setPosition(glm::vec2 position)
{
	if (position.x != -1.f)
		m_position.x = position.x;

	if (position.y != -1.f)
		m_position.y = position.y;

	if (m_uiProperties != nullptr)
		m_uiProperties->pos = m_position + m_offset;
}

void IUserInterface::setSize(glm::vec2 size)
{
	if (size.x != -1.f)
		m_size.x = size.x;

	if (size.y != -1.f)
		m_size.y = size.y;

	if (m_uiProperties != nullptr)
		m_uiProperties->size = m_size;
}

void IUserInterface::setOffset(glm::vec2 offset)
{
	m_offset = offset;
}

void IUserInterface::setEnabled(bool isEnabled)
{
	m_isEnabled = isEnabled;

	if (m_uiProperties != nullptr)
		m_uiProperties->isEnabled = isEnabled;
}

void IUserInterface::setDescription(const std::string &description)
{
	m_description = description;
}

glm::vec2 IUserInterface::getPosition() const
{
	return m_position;
}

glm::vec2 IUserInterface::getSize() const
{
	return m_size;
}

glm::vec4 IUserInterface::getColour() const
{
	return m_colour;
}

bool IUserInterface::hasBorder() const
{
	return m_hasBorder;
}

bool IUserInterface::isEnabled() const
{
	return m_isEnabled;
}

void IUserInterface::getDescription(InputManager &inputManager, 
	std::string &output) const
{
	output = isHovering(inputManager) ? m_description : output = "";
}

bool IUserInterface::isHovering(InputManager &inputManager) const
{
	glm::vec2 mousePos{ inputManager.getMousePos() };
	glm::vec2 pos{ m_position + m_offset };

	return (mousePos.x >= pos.x && mousePos.x <= pos.x + m_size.x &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + m_size.y);
}
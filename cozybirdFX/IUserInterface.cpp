#include "IUserInterface.h"

bool IUserInterface::m_isClicked{ false };

IUserInterface::IUserInterface(glm::vec2 position, glm::vec2 size,
	glm::vec4 colour, bool hasBorder) : m_position(position),
	m_size(size), m_colour(colour), m_hasBorder(hasBorder)
{
}

void IUserInterface::process(InputManager &inputManager)
{
	// Skip disabled elements.
	if (!m_isEnabled)
		return;

	handleInput(inputManager);
}

void IUserInterface::addToRenderer(UIRenderer &uRenderer,
	TextRenderer &tRenderer)
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

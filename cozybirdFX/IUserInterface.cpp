#include "IUserInterface.h"
#include "UIRenderer.h"

IUserInterface::IUserInterface(glm::vec2 position, glm::vec2 size,
	glm::vec4 colour, bool hasBorder) : m_position(position),
	m_size(size), m_colour(colour), m_hasBorder(hasBorder)
{
}

void IUserInterface::addToRenderer(UIRenderer *uRenderer, 
	TextRenderer *tRenderer)
{
	if (uRenderer != nullptr)
	{
		UIRenderer::Properties prop;
		prop.pos = m_position + m_offset;
		prop.size = m_size;
		prop.colour = m_colour;
		prop.hasBorder = m_hasBorder;
		uRenderer->addElement(prop);
	}
}

void IUserInterface::setOffset(glm::vec2 offset)
{
	m_offset = offset;
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

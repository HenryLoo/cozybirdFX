#include "UIContainer.h"
#include "UIRenderer.h"

UIContainer::UIContainer(glm::vec2 position, glm::vec2 size, glm::vec4 colour, 
	bool hasBorder) : m_position(position), m_size(size), m_colour(colour),
	m_hasBorder(hasBorder)
{

}

void UIContainer::handleInput(InputManager *inputManager)
{
	for (const auto &element : m_elements)
	{
		element->handleInput(inputManager);
	}
}

void UIContainer::addElement(std::shared_ptr<IUserInterface> element)
{
	m_elements.push_back(element);
}

void UIContainer::addToRenderer(UIRenderer *uRenderer)
{
	if (uRenderer != nullptr)
	{
		UIRenderer::Properties prop;
		prop.pos = m_position;
		prop.size = m_size;
		prop.colour = m_colour;
		prop.hasBorder = m_hasBorder;
		uRenderer->addElement(prop);
	}
}

glm::vec2 UIContainer::getPosition() const
{
	return m_position;
}

glm::vec2 UIContainer::getSize() const
{
	return m_size;
}

glm::vec4 UIContainer::getColour() const
{
	return m_colour;
}

bool UIContainer::hasBorder() const
{
	return m_hasBorder;
}

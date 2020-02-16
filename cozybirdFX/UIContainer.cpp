#include "UIContainer.h"
#include "UIRenderer.h"

UIContainer::UIContainer(glm::vec2 position, glm::vec2 size, glm::vec4 colour, 
	bool hasBorder) : IUserInterface(position, size, colour, hasBorder)
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
	element->setOffset(m_position + m_offset);
	m_elements.push_back(element);
}

void UIContainer::addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer)
{
	// Add this container to the renderer.
	IUserInterface::addToRenderer(uRenderer, tRenderer);

	// Add each element to the renderer.
	for (const auto &element : m_elements)
	{
		element->addToRenderer(uRenderer, tRenderer);
	}
}
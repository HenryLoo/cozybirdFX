#include "UIContainer.h"
#include "UIRenderer.h"

const float UIContainer::SPACE{ 16.f };

UIContainer::UIContainer(glm::vec2 position, glm::vec2 size, glm::vec4 colour, 
	bool hasBorder) : IUserInterface(position, size, colour, hasBorder)
{
	m_isFittingWidth = (size.x == -1.f);
	m_isFittingHeight = (size.y == -1.f);
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
	element->setOffset(m_position + m_innerOffset);
	m_elements.push_back(element);

	glm::vec2 size{ element->getSize() };
	m_innerOffset.x += size.x + SPACE;
	m_maxElementWidth = glm::max(m_maxElementWidth, m_innerOffset.x);
	m_maxElementHeight = glm::max(m_maxElementHeight, size.y);
	m_hasLineBreak = false;

	fitContents();
}

void UIContainer::addNewLine()
{
	addNewLine(SPACE);
}

void UIContainer::addNewHalfLine()
{
	addNewLine(SPACE / 2.f);
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

void UIContainer::setPosition(glm::vec2 position)
{
	glm::vec2 oldPos{ m_position };
	IUserInterface::setPosition(position);

	// Reposition the container elements.
	glm::vec2 diff{ m_position - oldPos };
	for (const auto &element : m_elements)
	{
		glm::vec2 elementPos{ element->getPosition() };
		element->setPosition(elementPos + diff);
	}
}

void UIContainer::setSize(glm::vec2 size)
{
	IUserInterface::setSize(size);
}

void UIContainer::fitContents()
{
	if (m_isFittingWidth)
		m_size.x = m_maxElementWidth;
	if (m_isFittingHeight)
		m_size.y = m_innerOffset.y + m_maxElementHeight + SPACE;
}

void UIContainer::addNewLine(float space)
{
	if (m_elements.size() == 0 || m_hasLineBreak)
		return;

	m_innerOffset.y += m_maxElementHeight + space;
	m_innerOffset.x = m_position.x + SPACE;
	m_maxElementHeight = 0;
	m_hasLineBreak = true;

	fitContents();
}

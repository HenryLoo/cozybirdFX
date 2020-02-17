//#include "UIList.h"
//#include "InputManager.h"
//#include "UIRenderer.h"
//
//#include <GLFW/glfw3.h>
//
//namespace
//{
//	const float VERTICAL_SPACE{ 16.f };
//	const float BAR_WIDTH{ 24.f };
//}
//
//UIList::UIList(glm::vec2 position, glm::vec2 size) :
//	UIContainer(position, size)
//{
//}
//
//void UIList::handleInput(InputManager *inputManager)
//{
//	UIContainer::handleInput(inputManager);
//
//	// Activate scroller if the mouse is inside the bar's bounds.
//	glm::vec2 mousePos{ inputManager->getMousePos() };
//	glm::vec2 pos{ m_position + m_offset };
//	pos.x += m_size.x;
//	if ((!IUserInterface::m_isClicked || m_isClicked) &&
//		((mousePos.x >= pos.x && mousePos.x <= pos.x + BAR_WIDTH &&
//		mousePos.y >= pos.y && mousePos.y <= pos.y + m_size.y) ||
//		m_isClicked) && inputManager->isMouseDown(GLFW_MOUSE_BUTTON_1))
//	{
//		IUserInterface::m_isClicked = m_isClicked = true;
//
//		float offset{ mousePos.y - pos.y };
//		int index{ static_cast<int>(offset / m_size.y * m_elements.size()) };
//		index = glm::clamp(index, 0, (int)m_elements.size() - 1);
//		if (m_firstIndex != index)
//		{
//			m_firstIndex = index;
//			updateHandle();
//		}
//	}
//
//	// Disable clicked flag on left click release.
//	if (m_isClicked && !inputManager->isMouseDown(GLFW_MOUSE_BUTTON_1))
//	{
//		IUserInterface::m_isClicked = m_isClicked = false;
//	}
//}
//
//void UIList::addElement(std::shared_ptr<IUserInterface> element)
//{
//	element->setOffset(m_position + m_innerOffset);
//	m_elements.push_back(element);
//
//	m_innerOffset.y += element->getSize().y + VERTICAL_SPACE;
//}
//
//void UIList::addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer)
//{
//	// Add this list to the renderer.
//	IUserInterface::addToRenderer(uRenderer, tRenderer);
//
//	// Add each element to the renderer.
//	float totalHeight{ 0.f };
//	for (const auto &element : m_elements)
//	{
//		// Don't draw elements if they exceed the list's height.
//		totalHeight += element->getSize().y + VERTICAL_SPACE;
//		//if (totalHeight >= m_size.y)
//			//break;
//
//		element->addToRenderer(uRenderer, tRenderer);
//	}
//
//	// Draw a scrollbar.
//	UIRenderer::Properties bar;
//	bar.pos = m_position + m_offset;
//	bar.pos.x += m_size.x;
//	bar.size = glm::vec2(BAR_WIDTH, m_size.y);
//	bar.colour = glm::vec4(0.2f, 0.2f, 0.2f, 1.f);
//	uRenderer->addElement(bar);
//
//	// Draw scroll handle.
//	UIRenderer::Properties handle;
//	handle.pos = m_position + m_offset;
//	handle.pos.x += m_size.x;
//	handle.size = glm::vec2(BAR_WIDTH,
//		m_size.y / m_innerOffset.y * m_size.y);
//	handle.colour = glm::vec4(0.7f, 0.7f, 0.9f, 1.f);
//	handle.hasBorder = true;
//	auto handleIt{ uRenderer->addElement(handle) };
//	m_barPosY = &handleIt->pos.y;
//}
//
//void UIList::updateHandle()
//{
//	if (m_barPosY != nullptr)
//	{
//		float incr{ (m_size.y - m_size.y / m_innerOffset.y * m_size.y) / 4 };
//		*m_barPosY = m_position.y + m_offset.y + m_firstIndex * incr;
//	}
//}

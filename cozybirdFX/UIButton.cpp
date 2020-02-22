#include "UIButton.h"
#include "InputManager.h"
#include "TextRenderer.h"

#include <GLFW/glfw3.h>

UIButton::UIButton(std::string label, std::function<void()> action,
	glm::vec2 size, glm::vec2 position, glm::vec4 colour, bool hasBorder) :
	IUserInterface(position, size, colour, hasBorder),
	m_label(label), m_action(action)
{
}

void UIButton::handleInput(InputManager *inputManager)
{
	// Call the action if left clicked inside the button's bounds.
	glm::vec2 mousePos{ inputManager->getMousePos() };
	glm::vec2 pos{ m_position + m_offset };
	if (mousePos.x >= pos.x && mousePos.x <= pos.x + m_size.x &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + m_size.y &&
		inputManager->isMouseDown(GLFW_MOUSE_BUTTON_1, true))
	{
		m_action();
	}
}

void UIButton::addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer)
{
	// Add this button to the renderer.
	IUserInterface::addToRenderer(uRenderer, tRenderer);

	// Add this button's text to the renderer.
	TextRenderer::Properties prop;
	prop.text = m_label;
	prop.pos = m_position + m_offset;
	prop.size = m_size;
	prop.isVerticalCenter = true;
	prop.align = TextRenderer::TextAlign::CENTER;
	tRenderer->addText(prop);
}
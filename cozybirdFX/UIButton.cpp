#include "UIButton.h"
#include "InputManager.h"

#include <GLFW/glfw3.h>

namespace
{
	const glm::vec4 BUTTON_COLOUR{ 0.3f, 0.3f, 0.3f, 1.f };
	const glm::vec4 TOGGLED_COLOUR{ 0.1f, 0.1f, 0.8f, 1.f };
}

UIButton::UIButton(std::string label, glm::vec2 size, bool isTogglable, 
	std::function<void()> action, glm::vec2 position, bool hasBorder) :
	IUserInterface(position, size, BUTTON_COLOUR, hasBorder),
	m_label(label), m_action(action), m_isTogglable(isTogglable)
{
}

bool UIButton::handleInput(InputManager &inputManager,
	UndoableAction &action)
{
	// Call the action if left clicked inside the button's bounds.
	bool oldToggleState{ m_isToggled };
	glm::vec2 mousePos{ inputManager.getMousePos() };
	glm::vec2 pos{ m_position + m_offset };
	bool isHovering{ IUserInterface::isHovering(inputManager) };
	if (isHovering && !IUserInterface::m_isAnyClicked &&
		inputManager.isMousePressed(GLFW_MOUSE_BUTTON_1))
	{
		bool wasToggled{ m_isToggled };
		if (m_isTogglable)
		{
			setToggled(!m_isToggled);
		}

		m_action();

		// If there was no change in toggle, then there is nothing to undo.
		if (((m_isTogglable && wasToggled != m_isToggled) || !m_isTogglable) && 
			m_undoableAction.undo != nullptr)
		{
			action = m_undoableAction;
			return true;
		}
	}

	return false;
}

void UIButton::addToRenderer(UIRenderer &uRenderer, TextRenderer &tRenderer)
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
	auto it{ tRenderer.addText(prop) };
	m_tProperties = &*it;
}

void UIButton::setPosition(glm::vec2 position)
{
	glm::vec2 oldPos{ m_position };
	IUserInterface::setPosition(position);

	// Reposition the button's label.
	glm::vec2 diff{ m_position - oldPos };
	if (m_tProperties != nullptr)
		m_tProperties->pos += diff;
}

void UIButton::setEnabled(bool isEnabled)
{
	IUserInterface::setEnabled(isEnabled);

	if (m_tProperties != nullptr)
		m_tProperties->isEnabled = isEnabled;
}

void UIButton::setAction(std::function<void()> action)
{
	m_action = action;
}

void UIButton::setToggled(bool isToggled)
{
	if (m_isTogglable)
	{
		m_isToggled = isToggled;
		m_uiProperties->colour = m_isToggled ? TOGGLED_COLOUR : BUTTON_COLOUR;
	}
}

bool UIButton::isToggled() const
{
	return m_isToggled;
}

void UIButton::setUndoAction(const std::function<void()> &undo)
{
	// Toggle the button (if applicable), before performing undo/redo.
	m_undoableAction.undo = [this, undo]()
	{
		setToggled(!isToggled());
		undo();
	};
	m_undoableAction.redo = [this]()
	{
		setToggled(!isToggled());
		m_action();
	};;
}

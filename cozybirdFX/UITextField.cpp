#include "UITextField.h"
#include "InputManager.h"
#include "TextRenderer.h"
#include "UIRenderer.h"

#include <GLFW/glfw3.h>

namespace
{
	const float TEXT_OFFSET{ 4.f };

	const glm::vec4 DEACTIVATED_COLOUR{ 0.1f, 0.1f, 0.1f, 1.f };
	const glm::vec4 ACTIVATED_COLOUR{ 0.1f, 0.1f, 0.6f, 1.f };
	const glm::vec4 ERROR_COLOUR{ 0.6f, 0.1f, 0.1f, 1.f };
}

UITextField::UITextField(std::string label, glm::vec2 position, glm::vec2 size) :
	IUserInterface(position, size, DEACTIVATED_COLOUR, true),
	m_label(label)
{
}

void UITextField::handleInput(InputManager *inputManager)
{
	glm::vec2 mousePos{ inputManager->getMousePos() };
	glm::vec2 pos{ m_position + m_offset };

	// Activate text field on left click, inside the field's bounds.
	if (mousePos.x >= pos.x && mousePos.x <= pos.x + m_size.x &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + m_size.y)
	{
		if (!IUserInterface::m_isClicked &&
			inputManager->isMouseDown(GLFW_MOUSE_BUTTON_1, true))
		{
			setActivation(true, inputManager);
		}
	}
	// Deactivate text field when clicking outside the field's bounds.
	else if (inputManager->isMouseDown(GLFW_MOUSE_BUTTON_1))
	{
		setActivation(false, inputManager);
	}

	// Handle text input if field is activated.
	if (m_isActivated)
	{
		// Erase the last character from the field.
		if (inputManager->isKeyDown(GLFW_KEY_BACKSPACE, true))
		{
			if (!m_value->empty())
				m_value->erase(m_value->end() - 1);
		}
		// Deactivate on enter.
		else if (inputManager->isKeyDown(GLFW_KEY_ENTER))
		{
			setActivation(false, inputManager);
		}
		// Read text input and append onto the field's value.
		else
		{
			std::string str;
			inputManager->flushText(str);
			*m_value += str;
		}
	}
}

void UITextField::addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer)
{
	glm::vec2 textPos{ m_position + m_offset };
	textPos.x += TEXT_OFFSET;
	glm::vec2 textBox{ m_size.x - (2 * TEXT_OFFSET), m_size.y };

	// Add the field's label to the renderer.
	TextRenderer::Properties label;
	label.text = m_label;
	label.pos = textPos;
	label.size = textBox;
	label.isVerticalCenter = true;
	tRenderer->addText(label);

	// Add the field's value to the renderer.
	TextRenderer::Properties val;
	val.pos = textPos;
	val.size = textBox;
	val.isVerticalCenter = true;
	val.align = TextRenderer::TextAlign::RIGHT;
	auto valIt{ tRenderer->addText(val) };
	m_value = &(valIt->text);

	// Add the field's bar to the renderer.
	UIRenderer::Properties bar;
	bar.pos = m_position + m_offset;
	bar.size = m_size;
	bar.colour = m_colour;
	bar.hasBorder = true;
	auto barIt{ uRenderer->addElement(bar) };
	m_uiColour = &(barIt->colour);
}

void UITextField::getValue(std::string &output) const
{
	output = *m_value;
}

void UITextField::getValue(int &output) const
{
	try
	{
		output = std::stoi(*m_value);
	}
	catch (std::invalid_argument &)
	{
		*m_uiColour = ERROR_COLOUR;
	}
}

void UITextField::getValue(double &output) const
{
	try
	{
		output = std::stod(*m_value);
	}
	catch (std::invalid_argument &)
	{
		*m_uiColour = ERROR_COLOUR;
	}
}

void UITextField::setActivation(bool isActivated, InputManager *inputManager)
{
	m_isActivated = isActivated;
	*m_uiColour = isActivated ? ACTIVATED_COLOUR : DEACTIVATED_COLOUR;
	inputManager->toggleTextInput(isActivated);
}

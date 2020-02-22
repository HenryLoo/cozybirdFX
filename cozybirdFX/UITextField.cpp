#include "UITextField.h"
#include "InputManager.h"
#include "TextRenderer.h"
#include "UIRenderer.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace
{
	const float TEXT_OFFSET{ 4.f };

	const glm::vec4 DEACTIVATED_COLOUR{ 0.1f, 0.1f, 0.1f, 1.f };
	const glm::vec4 ACTIVATED_COLOUR{ 0.1f, 0.1f, 0.6f, 1.f };
	const glm::vec4 ERROR_DEACTIVATED_COLOUR{ 0.6f, 0.1f, 0.1f, 1.f };
	const glm::vec4 ERROR_ACTIVATED_COLOUR{ 0.9f, 0.1f, 0.1f, 1.f };
}

UITextField::UITextField(std::string label, glm::vec2 size, 
	glm::vec2 position) :
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
			inputManager->isMouseDown(GLFW_MOUSE_BUTTON_1))
		{
			setActivation(true, inputManager);
		}
	}
	// Deactivate text field when clicking outside the field's bounds.
	else if (m_isActivated && inputManager->isMouseDown(GLFW_MOUSE_BUTTON_1))
	{
		setActivation(false, inputManager);
	}

	// Handle text input if field is activated.
	if (m_isActivated)
	{
		// Erase the last character from the field.
		if (inputManager->isKeyDown(GLFW_KEY_BACKSPACE, true))
		{
			if (!m_value.empty())
				m_value.erase(m_value.end() - 1);
		}
		// Deactivate on enter.
		else if (inputManager->isKeyDown(GLFW_KEY_ENTER))
		{
			setActivation(false, inputManager);
			m_isNewValue = true;
		}
		// Read text input and append onto the field's value.
		else
		{
			std::string str;
			inputManager->flushText(str);
			m_value += str;
		}

		updateUI();
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
	m_valueText = &(valIt->text);

	// Add the field's bar to the renderer.
	UIRenderer::Properties bar;
	bar.pos = m_position + m_offset;
	bar.size = m_size;
	bar.colour = m_colour;
	bar.hasBorder = true;
	auto barIt{ uRenderer->addElement(bar) };
	m_uiColour = &(barIt->colour);

	updateUI();
}

void UITextField::setValue(const std::string &value)
{
	m_value = value;
}

void UITextField::setValue(int value)
{
	m_value = std::to_string(value);
}

void UITextField::setValue(float value)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << value;
	std::string valueStr = ss.str();
	m_value = valueStr;
}

bool UITextField::getValue(std::string &output)
{
	output = m_value;
	bool isNew = m_isNewValue;
	m_isNewValue = false;
	return isNew;
}

bool UITextField::getValue(int &output)
{
	try
	{
		output = std::stoi(m_value);
		m_isError = false;
		bool isNew = m_isNewValue;
		m_isNewValue = false;
		return isNew;
	}
	catch (std::exception &)
	{
		m_isError = true;
		m_isNewValue = false;
		return false;
	}
}

bool UITextField::getValue(float &output)
{
	try
	{
		output = std::stof(m_value);
		m_isError = false;
		bool isNew = m_isNewValue;
		m_isNewValue = false;
		return isNew;
	}
	catch (std::exception &)
	{
		m_isError = true;
		m_isNewValue = false;
		return false;
	}
}

void UITextField::setActivation(bool isActivated, InputManager *inputManager)
{
	m_isActivated = isActivated;

	if (m_isError)
		*m_uiColour = isActivated ? ERROR_ACTIVATED_COLOUR : ERROR_DEACTIVATED_COLOUR;
	else
		*m_uiColour = isActivated ? ACTIVATED_COLOUR : DEACTIVATED_COLOUR;

	inputManager->toggleTextInput(isActivated);
}

void UITextField::updateUI()
{
	if (m_valueText != nullptr)
		*m_valueText = m_value;
}

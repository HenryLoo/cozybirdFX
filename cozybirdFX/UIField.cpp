#include "UIField.h"
#include "InputManager.h"
#include "UIRenderer.h"

#include <GLFW/glfw3.h>
#include <iostream>

namespace
{
	const float TEXT_OFFSET{ 4.f };

	const glm::vec4 DEACTIVATED_COLOUR{ 0.1f, 0.1f, 0.1f, 1.f };
	const glm::vec4 ACTIVATED_COLOUR{ 0.1f, 0.1f, 0.6f, 1.f };
	const glm::vec4 ERROR_DEACTIVATED_COLOUR{ 0.6f, 0.1f, 0.1f, 1.f };
	const glm::vec4 ERROR_ACTIVATED_COLOUR{ 0.9f, 0.1f, 0.1f, 1.f };
}

UIField::UIField(std::string label, glm::vec2 size, 
	glm::vec2 position) :
	IUserInterface(position, size, DEACTIVATED_COLOUR, true),
	m_label(label)
{
}

bool UIField::handleInput(InputManager &inputManager)
{
	glm::vec2 mousePos{ inputManager.getMousePos() };
	glm::vec2 pos{ m_position + m_offset };
	updateColour();

	// Activate text field on left click, inside the field's bounds.
	bool isHovering{ IUserInterface::isHovering(inputManager) };
	if (isHovering)
	{
		if (!IUserInterface::m_isAnyClicked &&
			inputManager.isMousePressed(GLFW_MOUSE_BUTTON_1))
		{
			setActivation(true, inputManager);
		}
	}
	// Deactivate text field when clicking outside the field's bounds.
	else if (m_isActivated && inputManager.isMousePressed(GLFW_MOUSE_BUTTON_1))
	{
		setActivation(false, inputManager);
		m_isNewValue = true;
	}

	// Handle text input if field is activated.
	if (m_isActivated)
	{
		// Erase the last character from the field.
		if (inputManager.isKeyPressed(GLFW_KEY_BACKSPACE))
		{
			if (!m_value.empty())
				m_value.erase(m_value.end() - 1);
		}
		// Deactivate on enter.
		else if (inputManager.isKeyPressed(GLFW_KEY_ENTER))
		{
			setActivation(false, inputManager);
			m_isNewValue = true;
			m_hasError = !formatValue();
		}
		// Read text input and append onto the field's value.
		else
		{
			std::string str;
			inputManager.flushText(str);
			m_value += str;
		}

		updateFieldValue();
	}

	return m_isNewValue;
}

void UIField::addToRenderer(UIRenderer &uRenderer, TextRenderer &tRenderer)
{
	// Add the text field's bar to the renderer.
	IUserInterface::addToRenderer(uRenderer, tRenderer);

	glm::vec2 textPos{ m_position + m_offset };
	textPos.x += TEXT_OFFSET;
	glm::vec2 textBox{ m_size.x - (2 * TEXT_OFFSET), m_size.y };

	// Add the field's label to the renderer.
	TextRenderer::Properties label;
	label.text = m_label;
	label.pos = textPos;
	label.size = textBox;
	label.isVerticalCenter = true;
	auto labelIt{ tRenderer.addText(label) };
	m_labelProperties = &*labelIt;

	// Add the field's value to the renderer.
	TextRenderer::Properties val;
	val.pos = textPos;
	val.size = textBox;
	val.isVerticalCenter = true;
	val.align = TextRenderer::TextAlign::RIGHT;
	auto valIt{ tRenderer.addText(val) };
	m_valProperties = &*valIt;

	updateFieldValue();
}

void UIField::setPosition(glm::vec2 position)
{
	glm::vec2 oldPos{ m_position };
	IUserInterface::setPosition(position);

	// Reposition the label.
	glm::vec2 diff{ m_position - oldPos };
	if (m_labelProperties != nullptr)
		m_labelProperties->pos += diff;

	// Reposition the value text.
	if (m_valProperties != nullptr)
		m_valProperties->pos += diff;
}

void UIField::setEnabled(bool isEnabled)
{
	IUserInterface::setEnabled(isEnabled);

	if (m_labelProperties != nullptr)
		m_labelProperties->isEnabled = isEnabled;

	if (m_valProperties != nullptr)
		m_valProperties->isEnabled = isEnabled;
}

void UIField::setValue(const std::string &value)
{
	m_value = value;
	m_isNewValue = true;
	m_hasError = !formatValue();
	updateFieldValue();
}

bool UIField::getValue(std::string &output)
{
	output = m_value;
	return isNewValue();
}

void UIField::updateFieldValue()
{
	if (m_valProperties != nullptr)
		m_valProperties->text = m_value;
}

bool UIField::isNewValue()
{
	bool isNew = m_isNewValue;
	m_isNewValue = false;
	return isNew;
}

void UIField::setActivation(bool isActivated, InputManager &inputManager)
{
	m_isActivated = isActivated;
	inputManager.toggleTextInput(isActivated);
}

bool UIField::formatValue()
{
	return true;
}

void UIField::updateColour()
{
	if (m_hasError)
		m_uiProperties->colour = m_isActivated ? ERROR_ACTIVATED_COLOUR : ERROR_DEACTIVATED_COLOUR;
	else
		m_uiProperties->colour = m_isActivated ? ACTIVATED_COLOUR : DEACTIVATED_COLOUR;
}

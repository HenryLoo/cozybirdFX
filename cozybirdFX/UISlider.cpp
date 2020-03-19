#include "UISlider.h"
#include "InputManager.h"
#include "UIRenderer.h"

#include <GLFW/glfw3.h>

namespace
{
	const float TEXT_OFFSET{ 4.f };
	const float FILL_OFFSET{ 2.f };

	const glm::vec4 BAR_COLOUR{ 0.1f, 0.1f, 0.1f, 1.f };
	const glm::vec4 FILL_COLOUR{ 0.5f, 0.5f, 0.5f, 1.f };
}

UISlider::UISlider(std::string label, glm::ivec2 range, 
	glm::vec2 size, glm::vec2 position) :
	IUserInterface(position, size, BAR_COLOUR, true),
	m_label(label), m_range(range)
{
}

void UISlider::handleInput(InputManager &inputManager)
{
	// Activate slider if the mouse is inside the bar's bounds.
	glm::vec2 mousePos{ inputManager.getMousePos() };
	glm::vec2 pos{ m_position + m_offset };
	if ((!IUserInterface::m_isClicked || m_isClicked) &&
		((mousePos.x >= pos.x && mousePos.x <= pos.x + m_size.x &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + m_size.y) || 
		m_isClicked) && inputManager.isMouseDown(GLFW_MOUSE_BUTTON_1))
	{
		IUserInterface::m_isClicked = m_isClicked = true;

		float offset{ mousePos.x - pos.x };
		int val{ static_cast<int>(offset / m_size.x * m_range.y) };
		if (m_value != val)
		{
			setValue(val);
			updateBar();
		}
	}

	// Disable clicked flag on left click release.
	if (m_isClicked && !inputManager.isMouseDown(GLFW_MOUSE_BUTTON_1))
	{
		IUserInterface::m_isClicked = m_isClicked = false;
	}
}

void UISlider::addToRenderer(UIRenderer &uRenderer, TextRenderer &tRenderer)
{
	// Add the slider's bar to the renderer.
	IUserInterface::addToRenderer(uRenderer, tRenderer);

	// Add the slider's fill to the renderer.
	UIRenderer::Properties fill;
	fill.pos = m_position + m_offset;
	fill.pos.x += FILL_OFFSET / 2;
	fill.pos.y += FILL_OFFSET / 2;
	fill.size.y = m_size.y - FILL_OFFSET;
	fill.colour = FILL_COLOUR;
	auto fillIt{ uRenderer.addElement(fill) };
	m_fillProperties = &*fillIt;

	glm::vec2 textPos{ m_position + m_offset };
	textPos.x += TEXT_OFFSET;
	glm::vec2 textBox{ m_size.x - (2 * TEXT_OFFSET), m_size.y };

	// Add the slider's label to the renderer.
	TextRenderer::Properties label;
	label.text = m_label;
	label.pos = textPos;
	label.size = textBox;
	label.isVerticalCenter = true;
	auto labelIt{ tRenderer.addText(label) };
	m_labelProperties = &*labelIt;

	// Add the slider's value to the renderer.
	TextRenderer::Properties val;
	val.pos = textPos;
	val.size = textBox;
	val.isVerticalCenter = true;
	val.align = TextRenderer::TextAlign::RIGHT;
	auto valIt{ tRenderer.addText(val) };
	m_valProperties = &*valIt;

	updateBar();
}

void UISlider::setPosition(glm::vec2 position)
{
	glm::vec2 oldPos{ m_position };
	IUserInterface::setPosition(position);

	// Reposition the fill.
	glm::vec2 diff{ m_position - oldPos };
	if (m_fillProperties != nullptr)
		m_fillProperties->pos += diff;

	// Reposition the label.
	if (m_labelProperties != nullptr)
		m_labelProperties->pos += diff;

	// Reposition the value text.
	if (m_valProperties != nullptr)
		m_valProperties->pos += diff;
}

void UISlider::setEnabled(bool isEnabled)
{
	IUserInterface::setEnabled(isEnabled);

	if (m_fillProperties != nullptr)
		m_fillProperties->isEnabled = isEnabled;

	if (m_labelProperties != nullptr)
		m_labelProperties->isEnabled = isEnabled;

	if (m_valProperties != nullptr)
		m_valProperties->isEnabled = isEnabled;
}

void UISlider::setValue(int value)
{
	m_value = glm::clamp(value, m_range.x, m_range.y);
	updateBar();
}

int UISlider::getValue() const
{
	return m_value;
}

void UISlider::updateBar()
{
	if (m_valProperties != nullptr)
		m_valProperties->text = std::to_string(m_value);

	if (m_fillProperties != nullptr)
		m_fillProperties->size.x = (float)m_value / m_range.y * (m_size.x - FILL_OFFSET);
}

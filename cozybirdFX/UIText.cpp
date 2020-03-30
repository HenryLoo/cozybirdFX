#include "UIText.h"

UIText::UIText(std::string text, glm::vec2 size, glm::vec2 position) :
	IUserInterface(position, size, glm::vec4(0.f), false), m_text(text)
{

}

bool UIText::handleInput(InputManager &inputManager,
	UndoableAction &action)
{
	// Do nothing.
	return false;
}

void UIText::addToRenderer(UIRenderer &uRenderer, TextRenderer &tRenderer)
{
	glm::vec2 textPos{ m_position + m_offset };

	// Add the label to the renderer.
	TextRenderer::Properties prop;
	prop.text = m_text;
	prop.pos = textPos;
	prop.size = m_size;
	prop.isVerticalCenter = true;
	auto it{ tRenderer.addText(prop) };
	m_tProperties = &*it;
}

void UIText::setPosition(glm::vec2 position)
{
	glm::vec2 oldPos{ m_position };
	IUserInterface::setPosition(position);

	// Reposition the label.
	glm::vec2 diff{ m_position - oldPos };
	if (m_tProperties != nullptr)
		m_tProperties->pos += diff;
}

void UIText::setEnabled(bool isEnabled)
{
	IUserInterface::setEnabled(isEnabled);

	if (m_tProperties != nullptr)
		m_tProperties->isEnabled = isEnabled;
}

void UIText::setText(const std::string &text)
{
	if (m_tProperties != nullptr)
		m_tProperties->text = text;
}

void UIText::setAlign(TextRenderer::TextAlign align)
{
	if (m_tProperties != nullptr)
		m_tProperties->align = align;
}

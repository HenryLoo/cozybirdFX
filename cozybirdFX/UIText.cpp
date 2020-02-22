#include "UIText.h"
#include "TextRenderer.h"

UIText::UIText(std::string text, glm::vec2 size, glm::vec2 position) :
	IUserInterface(position, size, glm::vec4(0.f), false), m_text(text)
{

}

void UIText::handleInput(InputManager *inputManager)
{
	// Do nothing.
}

void UIText::addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer)
{
	glm::vec2 textPos{ m_position + m_offset };

	// Add the label to the renderer.
	TextRenderer::Properties label;
	label.text = m_text;
	label.pos = textPos;
	label.size = m_size;
	tRenderer->addText(label);
}

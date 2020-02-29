#pragma once

#include "IUserInterface.h"
#include "TextRenderer.h"

#include <functional>
#include <string>

class UIButton : public IUserInterface
{
public:
	UIButton(std::string label, std::function<void()> action,
		glm::vec2 size, glm::vec2 position = { 0.f, 0.f },
		glm::vec4 colour = { 0.3f, 0.3f, 0.3f, 1.f }, bool hasBorder = true);

	virtual void handleInput(InputManager *inputManager);
	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);

	virtual void setPosition(glm::vec2 position);

private:
	// Call this function when the button is clicked.
	std::function<void()> m_action;

	// The button's label.
	std::string m_label;

	// Pointer to the label's text property.
	// This allows for dynamic changes.
	TextRenderer::Properties *m_tProperties;
};
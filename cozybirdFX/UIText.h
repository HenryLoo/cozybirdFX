#pragma once

#include "IUserInterface.h"

#include <glm/glm.hpp>

#include <string>

class UIText : public IUserInterface
{
public:
	UIText(std::string text, glm::vec2 size, glm::vec2 position = { 0.f, 0.f });

	virtual void handleInput(InputManager *inputManager);
	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);

private:
	// The label's text.
	std::string m_text;
};
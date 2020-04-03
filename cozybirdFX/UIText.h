#pragma once

#include "IUserInterface.h"
#include "TextRenderer.h"

#include <glm/glm.hpp>

#include <string>

class UIText : public IUserInterface
{
public:
	UIText(std::string text, glm::vec2 size, glm::vec2 position = { 0.f, 0.f });

	virtual void addToRenderer(SpriteRenderer &sRenderer, 
		UIRenderer &uRenderer, TextRenderer &tRenderer) override;

	virtual void setPosition(glm::vec2 position) override;
	virtual void setEnabled(bool isEnabled) override;

	void setText(const std::string &text);
	void setAlign(TextRenderer::TextAlign align);

protected:
	virtual bool handleInput(InputManager &inputManager,
		UndoableAction &action) override;

private:
	// The label's text.
	std::string m_text;

	// Pointer to the label's text property.
	// This allows for dynamic changes.
	TextRenderer::Properties *m_tProperties{ nullptr };
};
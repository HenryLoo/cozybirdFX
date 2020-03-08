#pragma once

#include "IUserInterface.h"
#include "TextRenderer.h"

#include <glm/glm.hpp>

#include <string>

class InputManager;

class UITextField : public IUserInterface
{
public:
	UITextField(std::string label, glm::vec2 size,
		glm::vec2 position = { 0.f, 0.f });

	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);

	virtual void setPosition(glm::vec2 position);
	virtual void setEnabled(bool isEnabled);

	// Set the current value.
	void setValue(const std::string &value);
	void setValue(int value);
	void setValue(float value);

	// Get the text field's current value.
	// Return whether or not this is a new value.
	bool getValue(std::string &output);
	bool getValue(int &output);
	bool getValue(float &output);

protected:
	virtual void handleInput(InputManager *inputManager);

private:
	void setActivation(bool isActivated, InputManager *inputManager);

	void updateUI();

	// The text field's label.
	std::string m_label;

	// Flag for if the text field is allowing text input.
	bool m_isActivated{ false };

	// The text field's current value.
	std::string m_value;

	// Flag for if there is invalid input.
	bool m_isError{ false };

	// Flag for if there is a new value to get.
	bool m_isNewValue{ true };

	// Hold pointers to its rendering items, so that their values can be
	// dynamically updated.
	TextRenderer::Properties *m_labelProperties{ nullptr };
	TextRenderer::Properties *m_valProperties{ nullptr };
};
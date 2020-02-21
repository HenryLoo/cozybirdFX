#pragma once

#include "IUserInterface.h"

#include <glm/glm.hpp>

#include <string>

class InputManager;

class UITextField : public IUserInterface
{
public:
	UITextField(std::string label, glm::vec2 position, glm::vec2 size);

	virtual void handleInput(InputManager *inputManager);
	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);

	// Get the text field's current value.
	void getValue(std::string &output) const;
	void getValue(int &output) const;
	void getValue(float &output) const;

private:
	void setActivation(bool isActivated, InputManager *inputManager);

	// The text field's label.
	std::string m_label;

	// Flag for if the text field is allowing text input.
	bool m_isActivated{ false };

	// The text field's current value.
	// This should point to the value in its UIRenderer property,
	// so that it can be updated directly.
	std::string *m_value{ nullptr };

	// Pointer to the UIRenderer property's colour.
	glm::vec4 *m_uiColour{ nullptr };
};
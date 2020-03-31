#pragma once

#include "IUserInterface.h"
#include "TextRenderer.h"

#include <glm/glm.hpp>

#include <string>

class InputManager;

class UIField : public IUserInterface
{
public:
	UIField(std::string label, glm::vec2 size,
		glm::vec2 position = { 0.f, 0.f });

	virtual void addToRenderer(SpriteRenderer &sRenderer, 
		UIRenderer &uRenderer, TextRenderer &tRenderer) override;

	virtual void setPosition(glm::vec2 position) override;
	virtual void setEnabled(bool isEnabled) override;

	// Set the current value.
	void setValue(const std::string &value);

	// Get the text field's current value.
	// Return whether or not this is a new value.
	bool getValue(std::string &output);

protected:
	virtual bool handleInput(InputManager &inputManager,
		UndoableAction &action) override;

	// The field's current value.
	std::string m_value;
	std::string m_oldValue;

	// Check if a new value was set and disable it.
	bool isNewValue();

private:
	void setActivation(bool isActivated, InputManager &inputManager);

	// Update the field's text.
	void updateFieldValue();

	// Return true if value was succesfully formatted.
	// Otherwise return  false.
	virtual bool formatValue();

	void updateColour();

	// The field's label.
	std::string m_label;

	// Flag for if the field is allowing input.
	bool m_isActivated{ false };

	// Flag for if there is invalid input.
	bool m_hasError{ false };

	// Flag for if there is a new value to get.
	bool m_isNewValue{ true };

	// Hold pointers to its rendering items, so that their values can be
	// dynamically updated.
	TextRenderer::Properties *m_labelProperties{ nullptr };
	TextRenderer::Properties *m_valProperties{ nullptr };
};
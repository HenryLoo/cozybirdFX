#pragma once

#include "IUserInterface.h"
#include "TextRenderer.h"

#include <functional>
#include <string>

class UIButton : public IUserInterface
{
public:
	UIButton(std::string label, glm::vec2 size, bool isTogglable = false,
		std::function<void()> action = []() {}, glm::vec2 position = { 0.f, 0.f },
		bool hasBorder = true);

	virtual void addToRenderer(UIRenderer &uRenderer, 
		TextRenderer &tRenderer) override;

	virtual void setPosition(glm::vec2 position) override;
	virtual void setEnabled(bool isEnabled) override;

	void setAction(std::function<void()> action);
	void setToggled(bool isToggled);
	bool isToggled() const;

	void setUndoAction(const std::function<void()> &undo);

protected:
	virtual bool handleInput(InputManager &inputManager, 
		UndoableAction &action) override;

private:
	// Call this function when the button is clicked.
	std::function<void()> m_action;

	// The button's label.
	std::string m_label;

	// Flag for if the button is a togglable button.
	bool m_isTogglable{ false };

	// Flag for if the button is toggled.
	bool m_isToggled{ false };

	// Pointer to the label's text property.
	// This allows for dynamic changes.
	TextRenderer::Properties *m_tProperties{ nullptr };

	// Each button may have a different undoable action.
	UndoableAction m_undoableAction;
};
#pragma once

#include "IUserInterface.h"

#include <glm/glm.hpp>

#include <string>

class UISlider : public IUserInterface
{
public:
	UISlider(std::string label, glm::ivec2 range,
		glm::vec2 size, glm::vec2 position = { 0.f, 0.f });

	virtual void handleInput(InputManager *inputManager);
	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);

	// Set the current value.
	void setValue(int value);

	// Get the current value.
	int getValue() const;

private:
	// Update the bar's value label and width.
	void updateBar();

	// The slider's label.
	std::string m_label;

	// The range of values for this slider.
	glm::ivec2 m_range{ 1 };

	// The current value for this slider.
	// This must be within the range.
	int m_value{ 0 };

	// Flag for if the left click is held down.
	// This allows for dragging the slider even if the cursor is not inside the
	// slider bar.
	bool m_isClicked{ false };

	// Hold pointers to its rendering items, so that their values can be
	// dynamically updated.
	float *m_barWidth{ nullptr };
	std::string *m_valueText{ nullptr };
};
#pragma once

#include "UIField.h"

class UIIntField : public UIField
{
public:
	UIIntField(std::string label, int maxChars, glm::vec2 size,
		glm::vec2 position = { 0.f, 0.f });

	// Set the current value.
	void setValue(int value);

	// Get the field's current value.
	// Return whether or not this is a new value.
	bool getValue(int &output);

private:
	virtual bool formatValue() override;
};
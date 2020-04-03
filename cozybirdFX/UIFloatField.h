#pragma once

#include "UIField.h"

class UIFloatField : public UIField
{
public:
	UIFloatField(std::string label, int maxChars, glm::vec2 size, 
		int precision = 1, glm::vec2 position = { 0.f, 0.f });

	// Set the current value.
	void setValue(float value);

	// Get the field's current value.
	// Return whether or not this is a new value.
	bool getValue(float &output);

private:
	virtual bool formatValue() override;

	// The number of decimal places.
	int m_precision{ 1 };
};
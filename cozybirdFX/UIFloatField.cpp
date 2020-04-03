#include "UIFloatField.h"

#include <iomanip>
#include <sstream>
#include <iostream>

UIFloatField::UIFloatField(std::string label, int maxChars, glm::vec2 size,
	int precision, glm::vec2 position) :
	UIField(label, maxChars, size, position), m_precision(precision)
{
}

void UIFloatField::setValue(float value)
{
	UIField::setValue(std::to_string(value));
}

bool UIFloatField::getValue(float &output)
{
	try
	{
		output = std::stof(m_value);
		return isNewValue();
	}
	catch (std::exception &)
	{
		return false;
	}
}

bool UIFloatField::formatValue()
{
	try
	{
		float val{ m_value.empty() ? 0.f : std::stof(m_value) };
		std::stringstream ss;
		ss << std::fixed << std::setprecision(m_precision) << val;
		m_value = ss.str();
		return true;
	}
	catch (std::exception &)
	{
		return false;
	}
}

#include "UIIntField.h"

UIIntField::UIIntField(std::string label, int maxChars, glm::vec2 size, 
	glm::vec2 position) :
	UIField(label, maxChars, size, position)
{
}

void UIIntField::setValue(int value)
{
	UIField::setValue(std::to_string(value));
}

bool UIIntField::getValue(int &output)
{
	try
	{
		output = std::stoi(m_value);
		return isNewValue();
	}
	catch (std::exception &)
	{
		return false;
	}
}

bool UIIntField::formatValue()
{
	try
	{
		int val{ m_value.empty() ? 0 : std::stoi(m_value) };
		m_value = std::to_string(val);
		return true;
	}
	catch (std::exception &)
	{
		return false;
	}
}

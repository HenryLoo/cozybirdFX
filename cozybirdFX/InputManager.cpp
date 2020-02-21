#include "InputManager.h"

#include <GLFW/glfw3.h>
#include <unicode/unistr.h>

#include <iostream>

glm::vec2 InputManager::m_mousePos;
std::array<bool, 2> InputManager::m_mouseStates;
std::unordered_map<int, bool> InputManager::m_keyStates;
std::string InputManager::m_textString;
bool InputManager::m_isTextInput;

InputManager::InputManager(GLFWwindow *window)
{
	glfwSetCursorPosCallback(window, MousePositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCharCallback(window, TextCallback);
}

bool InputManager::isMouseDown(int button, bool isReleaseOnCheck) const
{
	if (button < 0 || button >= m_mouseStates.size())
		return false;

	bool isDown{ m_mouseStates[button] != GLFW_RELEASE };
	if (isReleaseOnCheck)
		m_mouseStates[button] = GLFW_RELEASE;
	return isDown;
}

bool InputManager::isKeyDown(int key, bool isReleaseOnCheck) const
{
	auto it{ m_keyStates.find(key) };

	// State for this key was not found, so just return false.
	if (it == m_keyStates.end())
	{
		return false;
	}

	// Otherwise, return the state.
	bool isDown{ it->second };
	if (isReleaseOnCheck)
		it->second = GLFW_RELEASE;
	return isDown;
}

glm::vec2 InputManager::getMousePos() const
{
	return m_mousePos;
}

void InputManager::flushText(std::string &output)
{
	output = m_textString;
	m_textString = "";
}

void InputManager::toggleTextInput(bool isEnabled)
{
	m_isTextInput = isEnabled;
}

void InputManager::MousePositionCallback(GLFWwindow *window,
	double mouseX, double mouseY)
{
	m_mousePos.x = static_cast<float>(mouseX);
	m_mousePos.y = static_cast<float>(mouseY);
}

void InputManager::MouseButtonCallback(GLFWwindow *window, 
	int button, int action, int mods)
{
	if (button < 0 || button >= m_mouseStates.size())
		return;

	/*std::cout << "Mouse button: " << button << ", Action: " << action << 
		", x: " << m_mousePos.x << ", y: " << m_mousePos.y << std::endl;*/

	m_mouseStates[button] = (action != GLFW_RELEASE);
}

void InputManager::KeyCallback(GLFWwindow *window, int key, 
	int scancode, int action, int mods)
{
	// Do nothing if text input is enabled (except for some keys).
	if (m_isTextInput && key != GLFW_KEY_BACKSPACE && key != GLFW_KEY_ENTER)
		return;

	bool state{ action != GLFW_RELEASE };
	auto it{ m_keyStates.find(key) };

	//std::cout << "Key: " << key << ", Action: " << action << std::endl;

	// State for this key was not found, so insert it.
	if (it == m_keyStates.end())
	{
		m_keyStates.insert({ key, state });
		return;
	}

	// Otherwise, just update the state.
	it->second = state;
}

void InputManager::TextCallback(GLFWwindow *window, unsigned int codepoint)
{
	// Do nothing if text input is disabled.
	if (!m_isTextInput)
		return;

	// Otherwise, append the character to the text string after.
	// converting from Unicode code point to UTF-8.
	icu::UnicodeString uniStr((UChar32) codepoint);
	std::string character;
	uniStr.toUTF8String(character);
	m_textString += character;
}

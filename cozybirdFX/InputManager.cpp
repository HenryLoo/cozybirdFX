#include "InputManager.h"

#include <GLFW/glfw3.h>
#include <unicode/unistr.h>

#include <iostream>

glm::vec2 InputManager::m_mousePos;
std::vector<InputManager::State> InputManager::m_mouseStates;
std::vector<InputManager::State> InputManager::m_keyStates;
std::string InputManager::m_textString;
bool InputManager::m_isTextInput;

InputManager::InputManager(GLFWwindow *window)
{
	glfwSetCursorPosCallback(window, MousePositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCharCallback(window, TextCallback);

	m_mouseStates.resize(2);
	m_keyStates.resize(GLFW_KEY_LAST);
}

void InputManager::update()
{
	for (InputManager::State &state : m_mouseStates)
	{
		state.previousState = state.currentState;
	}

	for (InputManager::State &state : m_keyStates)
	{
		state.previousState = state.currentState;
	}
}

bool InputManager::isMouseDown(int button, int mods) const
{
	return isInputDown(m_mouseStates, button, mods);
}

bool InputManager::isMousePressed(int button, int mods) const
{
	return isInputPressed(m_mouseStates, button, mods);
}

glm::vec2 InputManager::getMousePos() const
{
	return m_mousePos;
}

bool InputManager::isKeyDown(int key, int mods, bool hasDelay) const
{
	bool isDown{ isInputDown(m_keyStates, key, mods) };
	if (hasDelay)
		m_keyStates[key].currentState = GLFW_RELEASE;

	return isDown;
}

bool InputManager::isKeyPressed(int key, int mods) const
{
	return isInputPressed(m_keyStates, key, mods);
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

	m_mouseStates[button].previousState = m_mouseStates[button].currentState;
	m_mouseStates[button].currentState = action;
	m_mouseStates[button].mods = mods;
}

void InputManager::KeyCallback(GLFWwindow *window, int key, 
	int scancode, int action, int mods)
{
	// Do nothing if text input is enabled (except for some keys).
	if (m_isTextInput && key != GLFW_KEY_BACKSPACE && key != GLFW_KEY_ENTER)
		return;

	if (key < 0 || key >= m_keyStates.size())
		return;

	//std::cout << "Key: " << key << ", Action: " << action << std::endl;

	m_keyStates[key].previousState = m_keyStates[key].currentState;
	m_keyStates[key].currentState = action;
	m_keyStates[key].mods = mods;
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

bool InputManager::isInputDown(const std::vector<State> &states, int input,
	int mods) const
{
	const auto &state{ states[input] };
	return (state.currentState == GLFW_PRESS || 
		state.currentState == GLFW_REPEAT) &&
		((state.mods & mods) || mods == 0);
}

bool InputManager::isInputPressed(const std::vector<State> &states, int input, 
	int mods) const
{
	const auto &state{ states[input] };
	return state.currentState == GLFW_PRESS &&
		state.previousState == GLFW_RELEASE &&
		((state.mods & mods) || mods == 0);
}
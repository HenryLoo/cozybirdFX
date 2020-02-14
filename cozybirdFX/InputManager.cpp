#include "InputManager.h"

#include <GLFW/glfw3.h>

#include <iostream>

glm::vec2 InputManager::m_mousePos;
std::array<bool, 2> InputManager::m_mouseStates;
std::unordered_map<int, bool> InputManager::m_keyStates;

InputManager::InputManager(GLFWwindow *window)
{
	glfwSetCursorPosCallback(window, MousePositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetKeyCallback(window, KeyCallBack);
}

bool InputManager::isMouseDown(int button) const
{
	if (button < 0 || button >= m_mouseStates.size())
		return false;

	return m_mouseStates[button] != GLFW_RELEASE;
}

bool InputManager::isKeyDown(int key) const
{
	int scanCode{ glfwGetKeyScancode(key) };
	auto it{ m_keyStates.find(scanCode) };

	// State for this key was not found, so just return false.
	if (it == m_keyStates.end())
	{
		return false;
	}

	// Otherwise, return the state.
	return it->second;
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

	std::cout << "Mouse button: " << button << ", Action: " << action << 
		", x: " << m_mousePos.x << ", y: " << m_mousePos.y << std::endl;

	m_mouseStates[button] = (action != GLFW_RELEASE);
}

void InputManager::KeyCallBack(GLFWwindow *window, int key, 
	int scancode, int action, int mods)
{
	bool state{ action != GLFW_RELEASE };
	auto it{ m_keyStates.find(key) };

	std::cout << "Key: " << key << ", Action: " << action << std::endl;

	// State for this key was not found, so insert it.
	if (it == m_keyStates.end())
	{
		m_keyStates.insert({ key, state });
		return;
	}

	// Otherwise, just update the state.
	it->second = state;
}

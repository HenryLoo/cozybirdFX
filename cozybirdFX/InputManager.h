#pragma once

#include <glm/glm.hpp>
#include <array>
#include <unordered_map>

struct GLFWwindow;

class InputManager
{
public:
	InputManager(GLFWwindow *window);

	// Check input states.
	bool isMouseDown(int button, bool isReleaseOnCheck = false) const;
	bool isKeyDown(int key, bool isReleaseOnCheck = false) const;
	glm::vec2 getMousePos() const;

	// Callback functions for GLFW.
	static void MousePositionCallback(GLFWwindow *window,
		double mouseX, double mouseY);

	static void MouseButtonCallback(GLFWwindow *window,
		int button, int action, int mods);

	static void KeyCallBack(GLFWwindow *window, int key, int scancode,
		int action, int mods);

private:
	// The mouse cursor's current position.
	static glm::vec2 m_mousePos;

	// Hold the current state for left/right mouse buttons.
	static std::array<bool, 2> m_mouseStates;

	// Hold the current state for all key inputs.
	static std::unordered_map<int, bool> m_keyStates;
};
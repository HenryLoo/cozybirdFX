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

	// Get the string result of the text callback so far, and then reset it.
	void flushText(std::string &output);

	// Enable/disable text input.
	void toggleTextInput(bool isEnabled);

	// Callback functions for GLFW.
	static void MousePositionCallback(GLFWwindow *window,
		double mouseX, double mouseY);

	static void MouseButtonCallback(GLFWwindow *window,
		int button, int action, int mods);

	static void KeyCallback(GLFWwindow *window, int key, int scancode,
		int action, int mods);

	static void TextCallback(GLFWwindow *window, unsigned int character);

private:
	// The mouse cursor's current position.
	static glm::vec2 m_mousePos;

	// Hold the current state for left/right mouse buttons.
	static std::array<bool, 2> m_mouseStates;

	// Hold the current state for all key inputs.
	static std::unordered_map<int, bool> m_keyStates;

	// Hold the string of character read by the text callback.
	static std::string m_textString;

	// Flag for if text input is enabled.
	static bool m_isTextInput;
};
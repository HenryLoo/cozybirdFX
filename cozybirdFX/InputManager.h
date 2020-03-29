#pragma once

#include <glm/glm.hpp>

#include <array>
#include <vector>

struct GLFWwindow;

class InputManager
{
public:
	struct State
	{
		int currentState;
		int previousState;
		int mods;
	};

	InputManager(GLFWwindow *window);

	void update();

	// Check input states.
	bool isMouseDown(int button, int mods = 0) const;
	bool isMousePressed(int button, int mods = 0) const;
	glm::vec2 getMousePos() const;
	bool isKeyDown(int key, int mods = 0) const;
	bool isKeyPressed(int key, int mods = 0) const;

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
	bool isInputDown(const std::vector<State> &states, int input, 
		int mods = 0) const;
	bool isInputPressed(const std::vector<State> &states, int input, 
		int mods = 0) const;

	// The mouse cursor's current position.
	static glm::vec2 m_mousePos;

	// Hold the current state for left/right mouse buttons.
	static std::vector<State> m_mouseStates;

	// Hold the current state for all key inputs.
	static std::vector<State> m_keyStates;

	// Hold the string of character read by the text callback.
	static std::string m_textString;

	// Flag for if text input is enabled.
	static bool m_isTextInput;
};
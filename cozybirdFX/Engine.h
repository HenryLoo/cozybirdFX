#pragma once

#include <GLFW/glfw3.h>

class Engine
{
public:
	Engine(GLFWwindow* window);
	~Engine();

	// Start the engine's loop.
	void start();

private:
	// Consult the input manager to read inputs.
	void handleInput();

	// Update all dynamic values.
	void update();

	// Consult all renderers to draw elements to the current scene.
	void render();

	// The application window.
	GLFWwindow* m_window;
};
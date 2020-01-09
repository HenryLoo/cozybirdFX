#include "Engine.h"

#include <GLFW/glfw3.h>
#include <memory>

int main()
{
    // Initialize GLFW.
    if (!glfwInit())
        return -1;

    // Create the window and its OpenGL context.
    GLFWwindow* window;
    window = glfwCreateWindow(1280, 720, "cozybirdFX", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Start the engine.
    std::unique_ptr<Engine> engine{ std::make_unique<Engine>(window) };
	if (engine != nullptr)
		engine->start();

    // Clean up.
    glfwTerminate();

	return 0;
}
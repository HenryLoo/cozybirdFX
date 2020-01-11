#include "Engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cout << "Main: Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window and its OpenGL context.
    GLFWwindow* window;
    const int WINDOW_WIDTH{ 1280 }, WINDOW_HEIGHT{ 720 };
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "cozybirdFX", NULL, NULL);
    if (!window)
    {
        std::cout << "Main: Failed to create window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Main: Failed to initialize GLAD." << std::endl;
        return -1;
    }
    
    // Set viewport and window resize callback.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Start the engine.
    std::unique_ptr<Engine> engine{ std::make_unique<Engine>(window) };
	if (engine != nullptr)
		engine->start();

    // Clean up.
    glfwTerminate();

	return 0;
}
#include "Engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

// Set the viewport to determine the size of the rendering window.
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    Engine *engine{ (Engine *)glfwGetWindowUserPointer(window) };
    if (engine != nullptr)
    {
        engine->updateNewWindowSize();
    }
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

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
    
    // Set viewport.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Disable depth testing.
    glDisable(GL_DEPTH_TEST);

    // Set window resize callback.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Set swap interval.
    glfwSwapInterval(1);

    // Set sticky keys to prevent missing polled input.
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    // Start the engine.
    std::unique_ptr<Engine> engine{ std::make_unique<Engine>(window) };
    glfwSetWindowUserPointer(window, engine.get());
	if (engine != nullptr)
		engine->start();

    // Clean up.
    glfwTerminate();

	return 0;
}
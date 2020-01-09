#include "Engine.h"

Engine::Engine(GLFWwindow* window) :
    m_window(window)
{

}

Engine::~Engine()
{

}

void Engine::start()
{
    // The main application loop.
    while (!glfwWindowShouldClose(m_window))
    {
        handleInput();
        update();
        render();
    }
}

void Engine::handleInput()
{
    glfwWaitEvents();

    // TODO: Integrate input manager here.
}

void Engine::update()
{
    // TODO: Update values here.
}

void Engine::render()
{
    // TODO: Integrate renderer functionality here.
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_window);
}

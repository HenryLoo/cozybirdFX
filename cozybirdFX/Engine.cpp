#include "Engine.h"
#include "SpriteRenderer.h"

#include <GLFW/glfw3.h>

Engine::Engine(GLFWwindow* window) :
    m_window(window)
{
    // Instantiate renderers.
    auto spriteRenderer{ std::make_unique<SpriteRenderer>() };
    m_renderers.push_back(std::move(spriteRenderer));
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
    for (const auto& renderer : m_renderers)
    {
        renderer->update();
    }
}

void Engine::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO: Integrate renderer functionality here.
    for (const auto& renderer : m_renderers)
    {
        renderer->render();
    }

    glfwSwapBuffers(m_window);
}

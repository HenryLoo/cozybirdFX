#include "Engine.h"
#include "SpriteRenderer.h"

#include <GLFW/glfw3.h>
#include <iostream>

Engine::Engine(GLFWwindow *window) :
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
    double lastFrame{ 0.f };

    // The main application loop.
    while (!glfwWindowShouldClose(m_window))
    {
        // If the window size was changed, update the renderer.
		if (m_hasNewWindowSize)
		{
			m_hasNewWindowSize = false;
			Renderer::updateWindowSize(m_window);
            std::cout << "UPDATE" << std::endl;
		}

        double currentFrame{ glfwGetTime() };
        float deltaTime{ static_cast<float>(currentFrame - lastFrame) };
        lastFrame = currentFrame;

        handleInput();
        update(deltaTime);
        render();
    }
}

void Engine::handleInput()
{
    glfwPollEvents();

    // TODO: Integrate input manager here.
}

void Engine::update(float deltaTime)
{
    // TODO: Update values here.
    for (const auto &renderer : m_renderers)
    {
        renderer->update(deltaTime);
    }
}

void Engine::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Integrate renderer functionality here.
    for (const auto &renderer : m_renderers)
    {
        renderer->render();
    }

    glfwSwapBuffers(m_window);
}

void Engine::updateNewWindowSize()
{
    m_hasNewWindowSize = true;
}

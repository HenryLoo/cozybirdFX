#include "Engine.h"
#include "SpriteRenderer.h"

#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "Emitter.h"

#include <GLFW/glfw3.h>
#include <iostream>

Engine::Engine(GLFWwindow *window) :
    m_window(window)
{
    // Initialize asset loader.
    m_assetLoader = std::make_unique<AssetLoader>();
    m_assetLoader->registerLoader<Texture>(new TextureLoader());
    m_assetLoader->registerLoader<Shader>(new ShaderLoader());

    // Instantiate entity manager.
    auto spriteRenderer{ std::make_unique<SpriteRenderer>(m_assetLoader.get()) };
    m_entityManager = std::make_unique<EntityManager>(spriteRenderer.get(), m_assetLoader.get());

    // Instantiate renderers.
    m_renderers.push_back(std::move(spriteRenderer));

    // Test emitter.
    m_emitter = std::make_unique<Emitter>(m_assetLoader.get());
    m_emitter->setTexture(m_assetLoader->load<Texture>("particle.png"));
    m_emitter->setNumToGenerate(30);
    m_emitter->setTimeToSpawn(0.5f);
    m_emitter->setVelocityMin({ -0.5f, -0.5f });
    m_emitter->setVelocityOffset({ 1.f, 1.f });
    m_emitter->setAcceleration({ 0.f, 0.f });
    m_emitter->setSize(0.5f);
    m_emitter->setColour({ 0.4f, 0.6f, 1.f });
    m_emitter->setDurationMin(2.f);
    m_emitter->setDurationOffset(0.3f);
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
		}

        double currentFrame{ glfwGetTime() };
        float deltaTime{ static_cast<float>(currentFrame - lastFrame) };
        lastFrame = currentFrame;

        handleInput();
        update(deltaTime);
        render(deltaTime);
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
    if (m_entityManager != nullptr)
        m_entityManager->update(deltaTime);

    for (const auto &renderer : m_renderers)
    {
        if (renderer != nullptr)
            renderer->update(deltaTime);
    }
}

void Engine::render(float deltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Integrate renderer functionality here.
    for (const auto &renderer : m_renderers)
    {
        if (renderer != nullptr)
            renderer->render();
    }

    m_emitter->update(deltaTime);
    m_emitter->render();

    glfwSwapBuffers(m_window);
}

void Engine::updateNewWindowSize()
{
    m_hasNewWindowSize = true;
}
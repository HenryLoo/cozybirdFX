#include "Engine.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"

#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "FontLoader.h"
#include "Font.h"
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
    m_assetLoader->registerLoader<Font>(new FontLoader());

    // Instantiate the input manager.
    m_inputManager = std::make_unique<InputManager>(m_window);

    // Instantiate renderers.
    auto spriteRenderer{ std::make_unique<SpriteRenderer>(m_assetLoader.get()) };
    auto textRenderer{ std::make_unique<TextRenderer>(m_assetLoader.get()) };

    // Instantiate entity manager.
    m_entityManager = std::make_unique<EntityManager>(spriteRenderer.get(), m_assetLoader.get());

    // TODO: Test emitter, remove this later.
    m_emitter = std::make_unique<Emitter>(m_assetLoader.get());
    m_emitter->setTexture(m_assetLoader->load<Texture>("particle.png"));
    m_emitter->setNumToGenerate(30);
    m_emitter->setTimeToSpawn(0.5f);
    m_emitter->setVelocityMin({ -32.f, -32.f });
    m_emitter->setVelocityOffset({ 64.f, 64.f });
    m_emitter->setAcceleration({ 0.f, 0.f });
    m_emitter->setSize(32.f);
    m_emitter->setColour({ 0.4f, 0.6f, 1.f });
    m_emitter->setDurationMin(2.f);
    m_emitter->setDurationOffset(0.3f);

    // TODO: Remove this later.
    m_font = m_assetLoader->load<Font>("default.ttf", 16);
    TextRenderer::Properties prop;
    prop.text = "Test message!";
    textRenderer->addText(m_font.get(), prop);

    // Add renderers to the list.
    m_renderers.push_back(std::move(spriteRenderer));
    m_renderers.push_back(std::move(textRenderer));
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
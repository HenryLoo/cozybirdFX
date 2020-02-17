#include "Engine.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "UIRenderer.h"

#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "FontLoader.h"
#include "Font.h"
#include "Emitter.h"
#include "UIContainer.h"
#include "UIButton.h"
#include "UISlider.h"

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
    auto uiRenderer{ std::make_unique<UIRenderer>(m_assetLoader.get()) };
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

    // TODO: Test UI, remove this later.
    glm::ivec2 windowSize;
    glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
    auto container{ std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, -1.f)) };
    auto button{ std::make_shared<UIButton>("Button", 
        []() { std::cout << "Clicked" << std::endl; },
        glm::vec2(0.f, 0.f), glm::vec2(100.f, 32.f)) };
    container->addElement(button);
    for (int i = 0; i < 10; ++i)
    {
        container->addNewLine();
        auto slider{ std::make_shared<UISlider>("Slider " + std::to_string(i), glm::vec2(0, 255),
        glm::vec2(0.f, 0.f), glm::vec2(255.f, 24.f)) };
        slider->setValue(50);
        container->addElement(slider);
    }
    container->addToRenderer(uiRenderer.get(), textRenderer.get());
    m_uiElements.push_back(container);

    // Add renderers to the list.
    m_renderers.push_back(std::move(uiRenderer));
    m_renderers.push_back(std::move(textRenderer));
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
    for (const auto &element : m_uiElements)
    {
        element->handleInput(m_inputManager.get());
    }
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
    glClearColor(0.f, 0.f, 0.f, 1.0);
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
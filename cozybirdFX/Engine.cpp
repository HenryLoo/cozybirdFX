#include "Engine.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "UIRenderer.h"

#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "FontLoader.h"
#include "Font.h"
#include "Emitter.h"
#include "EditorState.h"

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
    m_entityManager = std::make_unique<EntityManager>(spriteRenderer.get(), 
        m_assetLoader.get());

    // Instantiate the camera.
    glm::ivec2 windowSize{ getWindowSize() };
    m_camera = std::make_unique<Camera>(windowSize, 2.f);

    // TODO: Test emitter, remove this later.
    m_emitter = std::make_unique<Emitter>(m_assetLoader.get());
    m_emitter->setTexture(m_assetLoader->load<Texture>("particle.png"));

    // Default to editor state.
    EditorState *state{ new EditorState(textRenderer.get(), 
        uiRenderer.get()) };
    pushState(state);

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
            glm::ivec2 windowSize{ getWindowSize() };
            m_camera->setSize(windowSize);
        }

        double currentFrame{ glfwGetTime() };
        float deltaTime{ static_cast<float>(currentFrame - lastFrame) };
        lastFrame = currentFrame;

        handleInput();
        update(deltaTime);
        render(deltaTime);
    }
}

void Engine::pushState(IState *state)
{
    if (state == nullptr)
        return;

    m_states.push(std::move(std::unique_ptr<IState>(state)));
}

void Engine::popState()
{
    m_states.pop();
}

IState *Engine::getState() const
{
    if (m_states.empty())
        return nullptr;

    return m_states.top().get();
}

void Engine::handleInput()
{
    glfwPollEvents();

    // Handle input for the current state.
    IState *state{ getState() };
    if (state != nullptr)
        state->handleInput(m_inputManager.get());
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

    // Update the current state.
    IState *state{ getState() };
    if (state != nullptr)
        state->update(this, deltaTime);
}

void Engine::render(float deltaTime)
{
    glClearColor(0.f, 0.f, 0.f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call render for all renderers.
    for (const auto &renderer : m_renderers)
    {
        if (renderer != nullptr)
            renderer->render(m_camera.get());
    }

    m_emitter->update(deltaTime);
    m_emitter->render(m_camera.get());

    glfwSwapBuffers(m_window);
}

void Engine::updateNewWindowSize()
{
    m_hasNewWindowSize = true;
}

Emitter *Engine::getEmitter() const
{
    return m_emitter.get();
}

glm::ivec2 Engine::getWindowSize() const
{
    glm::ivec2 windowSize;
    glfwGetWindowSize(m_window, &windowSize.x, &windowSize.y);
    return windowSize;
}

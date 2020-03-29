#include "Engine.h"
#include "EmitterRenderer.h"
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

namespace
{
    const std::string WINDOW_TITLE{ "cozybirdFX" };
}

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

    // Instantiate entity manager.
    //m_entityManager = std::make_unique<EntityManager>(spriteRenderer.get(), 
    //    m_assetLoader.get());

    // Instantiate the camera.
    glm::ivec2 windowSize{ getWindowSize() };
    m_camera = std::make_unique<Camera>(windowSize, 1.f);

    // Default to editor state.
    EditorState *state{ new EditorState(*this, *m_assetLoader) };
    pushState(state);
}

Engine::~Engine()
{

}

void Engine::start()
{
    setWindowTitle();
    double lastFrame{ 0.f };

    // The main application loop.
    while (!glfwWindowShouldClose(m_window))
    {
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
    m_states.top()->enter();
}

void Engine::popState()
{
    m_states.pop();

    if (!m_states.empty())
        m_states.top()->enter();
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
        state->handleInput(*m_inputManager);

    m_inputManager->update();
}

void Engine::update(float deltaTime)
{
    // TODO: Update values here.
    //if (m_entityManager != nullptr)
    //    m_entityManager->update(deltaTime);

    // Update the current state.
    IState *state{ getState() };
    if (state != nullptr)
        state->update(deltaTime, *this);
}

void Engine::render(float deltaTime)
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the scene.
    IState *state{ getState() };
    if (state != nullptr)
        state->render(deltaTime, *m_camera);

    glfwSwapBuffers(m_window);
}

void Engine::updateNewWindowSize()
{
    glm::ivec2 windowSize{ getWindowSize() };
    m_camera->setSize(windowSize);
    updateViewport();
}

Camera &Engine::getCamera() const
{
    return *m_camera;
}

glm::ivec2 Engine::getWindowSize() const
{
    glm::ivec2 windowSize;
    glfwGetWindowSize(m_window, &windowSize.x, &windowSize.y);
    return windowSize;
}

void Engine::setWindowTitle(const std::string &title)
{
    std::string text{ title.empty() ? WINDOW_TITLE : 
        WINDOW_TITLE + " - " + title };
    glfwSetWindowTitle(m_window, text.c_str());
}

void Engine::updateViewport()
{
    glm::ivec2 windowSize{ getWindowSize() };
    glViewport(0, 0, windowSize.x, windowSize.y);
}

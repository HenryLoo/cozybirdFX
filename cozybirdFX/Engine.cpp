#include "Engine.h"
#include "SpriteRenderer.h"

#include "TextureLoader.h"
#include "ShaderLoader.h"

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
    m_entityManager = std::make_unique<EntityManager>(spriteRenderer.get());

    // Instantiate renderers.
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
        render();
    }
}

bool isF1Pressed{ false };
bool isF2Pressed{ false };
void Engine::handleInput()
{
    glfwPollEvents();

    // TODO: Integrate input manager here.


    // TODO: Test entity, remove this later.
    int f1State{ glfwGetKey(m_window, GLFW_KEY_F1) };
    int f2State{ glfwGetKey(m_window, GLFW_KEY_F2) };
    if (f1State && !isF1Pressed)
    {
        isF1Pressed = true;
        int id{ m_entityManager->createEntity() };
        std::cout << "Creating entity " << id << ", Num entities: " << 
            m_entityManager->getNumEntities() << std::endl;
        m_entityManager->addComponent(id, ECSComponent::COMPONENT_POSITION);
        m_entityManager->addComponent(id, ECSComponent::COMPONENT_SPRITE);
        m_entityManager->addComponent(id, ECSComponent::COMPONENT_TRANSFORM);
    }
    else if (!f1State)
    {
        isF1Pressed = false;
    }
    
    if (f2State && !isF2Pressed)
    {
        isF2Pressed = true;
        int numEntities{ m_entityManager->getNumEntities() };
        if (numEntities > 0)
        {
            int id{ rand() % numEntities };
            m_entityManager->deleteEntity(id);
            std::cout << "Deleting entity " << id << ", Num entities: " <<
                m_entityManager->getNumEntities() << std::endl;
        }
    }
    else if (!f2State)
    {
        isF2Pressed = false;
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

void Engine::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Integrate renderer functionality here.
    for (const auto &renderer : m_renderers)
    {
        if (renderer != nullptr)
            renderer->render();
    }

    glfwSwapBuffers(m_window);
}

void Engine::updateNewWindowSize()
{
    m_hasNewWindowSize = true;
}
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
#include "UITextField.h"

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

    // TODO: Test UI, remove this later.
    glm::ivec2 windowSize;
    glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
    auto container{ std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, -1.f)) };
    auto button{ std::make_shared<UIButton>("Button", 
        []() { std::cout << "Clicked" << std::endl; },
        glm::vec2(0.f, 0.f), glm::vec2(100.f, 32.f)) };
    container->addElement(button);

    container->addNewLine();
    m_rSlider = std::make_shared<UISlider>("R", glm::vec2(0, 255),
        glm::vec2(0.f, 0.f), glm::vec2(356.f, 24.f));
    m_rSlider->setValue(50);
    container->addElement(m_rSlider);

    container->addNewLine();
    m_gSlider = std::make_shared<UISlider>("G", glm::vec2(0, 255),
        glm::vec2(0.f, 0.f), glm::vec2(356.f, 24.f));
    m_gSlider->setValue(50);
    container->addElement(m_gSlider);

    container->addNewLine();
    m_bSlider = std::make_shared<UISlider>("B", glm::vec2(0, 255),
        glm::vec2(0.f, 0.f), glm::vec2(356.f, 24.f));
    m_bSlider->setValue(50);
    container->addElement(m_bSlider);

    container->addNewLine();
    m_xField = std::make_shared<UITextField>("x", glm::vec2(0.f, 0.f), 
        glm::vec2(170.f, 24.f));
    container->addElement(m_xField);

    m_yField = std::make_shared<UITextField>("y", glm::vec2(0.f, 0.f),
        glm::vec2(170.f, 24.f));
    container->addElement(m_yField);

    container->addNewLine();
    m_numGenField = std::make_shared<UITextField>("Particle Density", glm::vec2(0.f, 0.f),
        glm::vec2(356.f, 24.f));
    container->addElement(m_numGenField);

    container->addNewLine();
    m_spawnTimeField = std::make_shared<UITextField>("Time to Spawn (s)", glm::vec2(0.f, 0.f),
        glm::vec2(356.f, 24.f));
    container->addElement(m_spawnTimeField);

    container->addNewLine();
    m_sizeField = std::make_shared<UITextField>("Particle Size", glm::vec2(0.f, 0.f),
        glm::vec2(356.f, 24.f));
    container->addElement(m_sizeField);

    container->addNewLine();
    m_durationMinField = std::make_shared<UITextField>("Min Duration", glm::vec2(0.f, 0.f),
        glm::vec2(220.f, 24.f));
    container->addElement(m_durationMinField);

    m_durationOffsetField = std::make_shared<UITextField>("Offset", glm::vec2(0.f, 0.f),
        glm::vec2(120.f, 24.f));
    container->addElement(m_durationOffsetField);

    container->addNewLine();
    m_velMinXField = std::make_shared<UITextField>("Min Vel x", glm::vec2(0.f, 0.f),
        glm::vec2(220.f, 24.f));
    container->addElement(m_velMinXField);

    m_velOffsetXField = std::make_shared<UITextField>("Offset", glm::vec2(0.f, 0.f),
        glm::vec2(120.f, 24.f));
    container->addElement(m_velOffsetXField);

    container->addNewLine();
    m_velMinYField = std::make_shared<UITextField>("Min Vel y", glm::vec2(0.f, 0.f),
        glm::vec2(220.f, 24.f));
    container->addElement(m_velMinYField);

    m_velOffsetYField = std::make_shared<UITextField>("Offset", glm::vec2(0.f, 0.f),
        glm::vec2(120.f, 24.f));
    container->addElement(m_velOffsetYField);

    container->addNewLine();
    m_accelXField = std::make_shared<UITextField>("Accel x", glm::vec2(0.f, 0.f),
        glm::vec2(170.f, 24.f));
    container->addElement(m_accelXField);

    m_accelYField = std::make_shared<UITextField>("Accel y", glm::vec2(0.f, 0.f),
        glm::vec2(170.f, 24.f));
    container->addElement(m_accelYField);

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

    // Update emitter with UI values.

    // Set colour.
    float red{ m_rSlider->getValue() / 255.f };
    float green{ m_gSlider->getValue() / 255.f };
    float blue{ m_bSlider->getValue() / 255.f };
    m_emitter->setColour(glm::vec3(red, green, blue));

    // Set position.
    float x{ 0 };
    m_xField->getValue(x);
    float y{ 0 };
    m_yField->getValue(y);
    m_emitter->setPosition({ x, y });

    // Set number of particles to generate.
    int numToGen{ 30 };
    m_numGenField->getValue(numToGen);
    m_emitter->setNumToGenerate(numToGen);

    // Set spawn time.
    float spawnTime{ 0.5f };
    m_spawnTimeField->getValue(spawnTime);
    m_emitter->setTimeToSpawn(spawnTime);

    // Set velocity.
    glm::vec2 vel{ -32.f, -32.f };
    glm::vec2 velOffset{ 64.f, 64.f };
    m_velMinXField->getValue(vel.x);
    m_velMinYField->getValue(vel.y);
    m_velOffsetXField->getValue(velOffset.x);
    m_velOffsetYField->getValue(velOffset.x);
    m_emitter->setVelocityMin(vel);
    m_emitter->setVelocityOffset(velOffset);

    // Set acceleration.
    glm::vec2 accel{ 0.f };
    m_accelXField->getValue(accel.x);
    m_accelYField->getValue(accel.y);
    m_emitter->setAcceleration(accel);

    // Set particle size.
    float size{ 32.f };
    m_sizeField->getValue(size);
    m_emitter->setSize(size);

    // Set duration.
    float duration{ 2.f };
    float durationOffset{ 0.3f };
    m_durationMinField->getValue(duration);
    m_durationOffsetField->getValue(durationOffset);
    m_emitter->setDurationMin(duration);
    m_emitter->setDurationOffset(durationOffset);
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
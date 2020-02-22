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
#include "UIText.h"

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
    //auto button{ std::make_shared<UIButton>("Button", 
    //    []() { std::cout << "Clicked" << std::endl; },
    //    glm::vec2(0.f, 0.f), glm::vec2(100.f, 32.f)) };
    //container->addElement(button);

    auto colourLabel{ std::make_shared<UIText>("Colour", glm::vec2(200, 16.f)) };
    container->addElement(colourLabel);

    container->addNewHalfLine();
    m_rSlider = std::make_shared<UISlider>("R", glm::vec2(0, 255),
        glm::vec2(356.f, 24.f));
    m_rSlider->setValue(50);
    container->addElement(m_rSlider);

    container->addNewHalfLine();
    m_gSlider = std::make_shared<UISlider>("G", glm::vec2(0, 255), 
        glm::vec2(356.f, 24.f));
    m_gSlider->setValue(50);
    container->addElement(m_gSlider);

    container->addNewHalfLine();
    m_bSlider = std::make_shared<UISlider>("B", glm::vec2(0, 255),
        glm::vec2(356.f, 24.f));
    m_bSlider->setValue(50);
    container->addElement(m_bSlider);

    container->addNewLine();
    auto posLabel{ std::make_shared<UIText>("Position", glm::vec2(200, 16.f)) };
    container->addElement(posLabel);

    container->addNewHalfLine();
    m_xField = std::make_shared<UITextField>("x", glm::vec2(170.f, 24.f));
    m_xField->setValue(0.f);
    container->addElement(m_xField);

    m_yField = std::make_shared<UITextField>("y", glm::vec2(170.f, 24.f));
    m_yField->setValue(0.f);
    container->addElement(m_yField);

    container->addNewLine();
    m_numGenField = std::make_shared<UITextField>("Particle Density", 
        glm::vec2(356.f, 24.f));
    m_numGenField->setValue(30);
    container->addElement(m_numGenField);

    container->addNewLine();
    m_spawnTimeField = std::make_shared<UITextField>("Time to Spawn (s)", 
        glm::vec2(356.f, 24.f));
    m_spawnTimeField->setValue(0.5f);
    container->addElement(m_spawnTimeField);

    container->addNewLine();
    m_sizeField = std::make_shared<UITextField>("Particle Size",
        glm::vec2(356.f, 24.f));
    m_sizeField->setValue(32.f);
    container->addElement(m_sizeField);

    container->addNewLine();
    auto durationLabel{ std::make_shared<UIText>("Duration", glm::vec2(200, 16.f)) };
    container->addElement(durationLabel);

    container->addNewHalfLine();
    m_durationMinField = std::make_shared<UITextField>("Min",
        glm::vec2(170.f, 24.f));
    m_durationMinField->setValue(2.f);
    container->addElement(m_durationMinField);

    m_durationMaxField = std::make_shared<UITextField>("Max",
        glm::vec2(170.f, 24.f));
    m_durationMaxField->setValue(2.3f);
    container->addElement(m_durationMaxField);

    container->addNewLine();
    auto velXLabel{ std::make_shared<UIText>("Velocity.x", glm::vec2(200, 16.f)) };
    container->addElement(velXLabel);

    container->addNewHalfLine();
    m_velXMinField = std::make_shared<UITextField>("Min", glm::vec2(170.f, 24.f));
    m_velXMinField->setValue(-32.f);
    container->addElement(m_velXMinField);

    m_velXMaxField = std::make_shared<UITextField>("Max", glm::vec2(170.f, 24.f));
    m_velXMaxField->setValue(32.f);
    container->addElement(m_velXMaxField);

    container->addNewLine();
    auto velYLabel{ std::make_shared<UIText>("Velocity.y", glm::vec2(200, 16.f)) };
    container->addElement(velYLabel);

    container->addNewHalfLine();
    m_velYMinField = std::make_shared<UITextField>("Min", glm::vec2(170.f, 24.f));
    m_velYMinField->setValue(-32.f);
    container->addElement(m_velYMinField);

    m_velYMaxField = std::make_shared<UITextField>("Max", glm::vec2(170.f, 24.f));
    m_velYMaxField->setValue(32.f);
    container->addElement(m_velYMaxField);

    container->addNewLine();
    auto accelLabel{ std::make_shared<UIText>("Acceleration", glm::vec2(200, 16.f)) };
    container->addElement(accelLabel);

    container->addNewHalfLine();
    m_accelXField = std::make_shared<UITextField>("x", glm::vec2(170.f, 24.f));
    m_accelXField->setValue(0.f);
    container->addElement(m_accelXField);

    m_accelYField = std::make_shared<UITextField>("y", glm::vec2(170.f, 24.f));
    m_accelYField->setValue(0.f);
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
    glm::vec2 pos;
    bool isPosX{ m_xField->getValue(pos.x) };
    bool isPosY{ m_yField->getValue(pos.y) };
    if (isPosX || isPosY)
        m_emitter->setPosition(pos);

    // Set number of particles to generate.
    int numToGen;
    if (m_numGenField->getValue(numToGen))
        m_emitter->setNumToGenerate(numToGen);

    // Set spawn time.
    float spawnTime;
    if (m_spawnTimeField->getValue(spawnTime))
        m_emitter->setTimeToSpawn(spawnTime);

    // Set velocity.
    glm::vec2 velMin;
    bool isVelXMin{ m_velXMinField->getValue(velMin.x) };
    bool isVelYMin{ m_velYMinField->getValue(velMin.y) };
    if (isVelXMin || isVelYMin)
        m_emitter->setVelocityMin(velMin);

    // Convert max to offset.
    glm::vec2 velMax;
    bool isVelXMax{ m_velXMaxField->getValue(velMax.x) };
    bool isVelYMax{ m_velYMaxField->getValue(velMax.y) };
    if (isVelXMin || isVelYMin || isVelXMax || isVelYMax)
    {
        glm::vec2 velOffset{ velMax.x - velMin.x, velMax.y - velMin.y };
        velOffset = glm::clamp(velOffset, { 0.f, 0.f }, velOffset);
        std::cout << velOffset.x << ", " << velOffset.y << std::endl;
        m_emitter->setVelocityOffset(velOffset);
    }

    // Set acceleration.
    glm::vec2 accel;
    bool isAccelX{ m_accelXField->getValue(accel.x) };
    bool isAccelY{ m_accelYField->getValue(accel.y) };
    if (isAccelX || isAccelY)
        m_emitter->setAcceleration(accel);

    // Set particle size.
    float size;
    if (m_sizeField->getValue(size))
        m_emitter->setSize(size);

    // Set duration.
    float durationMin;
    bool isDurationMin{ m_durationMinField->getValue(durationMin) };
    if (isDurationMin)
        m_emitter->setDurationMin(durationMin);

    // Convert max to offset.
    float durationMax;
    bool isDurationMax{ m_durationMaxField->getValue(durationMax) };
    if (isDurationMin || isDurationMax)
    {
        float durationOffset{ durationMax - durationMin };
        durationOffset = glm::clamp(durationOffset, 0.f, durationOffset);
        m_emitter->setDurationOffset(durationOffset);
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
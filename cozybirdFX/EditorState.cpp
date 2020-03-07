#include "EditorState.h"
#include "Camera.h"
#include "Engine.h"
#include "Texture.h"

#include "UIContainer.h"
#include "UIButton.h"
#include "UISlider.h"
#include "UITextField.h"
#include "UIText.h"

#include <GLFW/glfw3.h>


EditorState::EditorState(Engine *engine, TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    m_topLeftPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, -1.f));

    glm::vec2 buttonSize{ glm::vec2(100.f, 32.f) };
    auto fileButton{ std::make_shared<UIButton>("File", 
        []()
        { 
            std::cout << "File" << std::endl;
        },
        buttonSize) };
    m_topLeftPanel->addElement(fileButton);

    auto exportButton{ std::make_shared<UIButton>("Export",
        [engine]()
        { 
            std::cout << "Export" << std::endl;
            engine->exportSpriteSheet();
        },
        buttonSize) };
    m_topLeftPanel->addElement(exportButton);

    auto playButton{ std::make_shared<UIButton>("Play",
        []() { std::cout << "Play" << std::endl; },
        buttonSize) };
    m_topLeftPanel->addElement(playButton);

    m_topLeftPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_topLeftPanel);

    m_topRightPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, -1.f));

    auto particlesButton{ std::make_shared<UIButton>("Particles",
        []()
        {
            std::cout << "Particles" << std::endl;
        },
        buttonSize) };
    m_topRightPanel->addElement(particlesButton);

    auto visualsButton{ std::make_shared<UIButton>("Visuals",
        []()
        {
            std::cout << "Visuals" << std::endl;
        },
        buttonSize) };
    m_topRightPanel->addElement(visualsButton);

    auto movementButton{ std::make_shared<UIButton>("Movement",
        []()
        {
            std::cout << "Movement" << std::endl;
        },
        buttonSize) };
    m_topRightPanel->addElement(movementButton);

    glm::vec2 topSize{ m_topLeftPanel->getSize() };

    m_topRightPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_topRightPanel);

    m_sidePanel = std::make_shared<UIContainer>(glm::vec2(0.f, topSize.y + 1.f),
        glm::vec2(-1.f, 0.f));

    auto colourLabel{ std::make_shared<UIText>("Colour", glm::vec2(200, 16.f)) };
    m_sidePanel->addElement(colourLabel);

    m_sidePanel->addNewHalfLine();
    m_rSlider = std::make_shared<UISlider>("R", glm::vec2(0, 255),
        glm::vec2(356.f, 24.f));
    m_rSlider->setValue(50);
    m_sidePanel->addElement(m_rSlider);

    m_sidePanel->addNewHalfLine();
    m_gSlider = std::make_shared<UISlider>("G", glm::vec2(0, 255),
        glm::vec2(356.f, 24.f));
    m_gSlider->setValue(50);
    m_sidePanel->addElement(m_gSlider);

    m_sidePanel->addNewHalfLine();
    m_bSlider = std::make_shared<UISlider>("B", glm::vec2(0, 255),
        glm::vec2(356.f, 24.f));
    m_bSlider->setValue(50);
    m_sidePanel->addElement(m_bSlider);

    m_sidePanel->addNewLine();
    auto posLabel{ std::make_shared<UIText>("Position", glm::vec2(200, 16.f)) };
    m_sidePanel->addElement(posLabel);

    m_sidePanel->addNewHalfLine();
    m_xField = std::make_shared<UITextField>("x", glm::vec2(170.f, 24.f));
    m_xField->setValue(0.f);
    m_sidePanel->addElement(m_xField);

    m_yField = std::make_shared<UITextField>("y", glm::vec2(170.f, 24.f));
    m_yField->setValue(0.f);
    m_sidePanel->addElement(m_yField);

    m_sidePanel->addNewLine();
    m_numGenField = std::make_shared<UITextField>("Particle Density",
        glm::vec2(356.f, 24.f));
    m_numGenField->setValue(30);
    m_sidePanel->addElement(m_numGenField);

    m_sidePanel->addNewLine();
    m_spawnTimeField = std::make_shared<UITextField>("Time to Spawn (s)",
        glm::vec2(356.f, 24.f));
    m_spawnTimeField->setValue(0.5f);
    m_sidePanel->addElement(m_spawnTimeField);

    m_sidePanel->addNewLine();
    m_sizeField = std::make_shared<UITextField>("Particle Size",
        glm::vec2(356.f, 24.f));
    m_sizeField->setValue(32.f);
    m_sidePanel->addElement(m_sizeField);

    m_sidePanel->addNewLine();
    auto durationLabel{ std::make_shared<UIText>("Duration", glm::vec2(200, 16.f)) };
    m_sidePanel->addElement(durationLabel);

    m_sidePanel->addNewHalfLine();
    m_durationMinField = std::make_shared<UITextField>("Min",
        glm::vec2(170.f, 24.f));
    m_durationMinField->setValue(2.f);
    m_sidePanel->addElement(m_durationMinField);

    m_durationMaxField = std::make_shared<UITextField>("Max",
        glm::vec2(170.f, 24.f));
    m_durationMaxField->setValue(2.3f);
    m_sidePanel->addElement(m_durationMaxField);

    m_sidePanel->addNewLine();
    auto velXLabel{ std::make_shared<UIText>("Velocity.x", glm::vec2(200, 16.f)) };
    m_sidePanel->addElement(velXLabel);

    m_sidePanel->addNewHalfLine();
    m_velXMinField = std::make_shared<UITextField>("Min", glm::vec2(170.f, 24.f));
    m_velXMinField->setValue(-32.f);
    m_sidePanel->addElement(m_velXMinField);

    m_velXMaxField = std::make_shared<UITextField>("Max", glm::vec2(170.f, 24.f));
    m_velXMaxField->setValue(32.f);
    m_sidePanel->addElement(m_velXMaxField);

    m_sidePanel->addNewLine();
    auto velYLabel{ std::make_shared<UIText>("Velocity.y", glm::vec2(200, 16.f)) };
    m_sidePanel->addElement(velYLabel);

    m_sidePanel->addNewHalfLine();
    m_velYMinField = std::make_shared<UITextField>("Min", glm::vec2(170.f, 24.f));
    m_velYMinField->setValue(-32.f);
    m_sidePanel->addElement(m_velYMinField);

    m_velYMaxField = std::make_shared<UITextField>("Max", glm::vec2(170.f, 24.f));
    m_velYMaxField->setValue(32.f);
    m_sidePanel->addElement(m_velYMaxField);

    m_sidePanel->addNewLine();
    auto accelLabel{ std::make_shared<UIText>("Acceleration", glm::vec2(200, 16.f)) };
    m_sidePanel->addElement(accelLabel);

    m_sidePanel->addNewHalfLine();
    m_accelXField = std::make_shared<UITextField>("x", glm::vec2(170.f, 24.f));
    m_accelXField->setValue(0.f);
    m_sidePanel->addElement(m_accelXField);

    m_accelYField = std::make_shared<UITextField>("y", glm::vec2(170.f, 24.f));
    m_accelYField->setValue(0.f);
    m_sidePanel->addElement(m_accelYField);

    m_sidePanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_sidePanel);

    m_bottomPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f), 
        glm::vec2(0.f, 0.f));
    m_bottomPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_bottomPanel);
}

void EditorState::handleInput(InputManager *inputManager)
{
    for (const auto &element : m_uiElements)
    {
        element->handleInput(inputManager);
    }
}

void EditorState::update(Engine *engine, float deltaTime)
{
    if (engine == nullptr)
        return;

    // If window size changed, rescale UI.
    glm::vec2 windowSize{ engine->getWindowSize() };
    if (m_windowSize != windowSize)
    {
        m_windowSize = windowSize;

        glm::vec2 tlSize{ m_topLeftPanel->getSize() };
        glm::vec2 trSize{ m_topRightPanel->getSize() };
        m_sidePanel->setSize(glm::vec2(-1.f, windowSize.y - tlSize.y - trSize.y - 2.f));
        glm::vec2 sideSize{ m_sidePanel->getSize() };
        m_sidePanel->setPosition(glm::vec2(windowSize.x - sideSize.x, -1.f));

        m_topLeftPanel->setSize(glm::vec2(windowSize.x - sideSize.x - 1.f, -1.f));
        m_topRightPanel->setSize(glm::vec2(sideSize.x, -1.f));
        glm::vec2 tlPos{ m_topLeftPanel->getPosition() };
        glm::vec2 sidePos{ m_sidePanel->getPosition() };
        m_topRightPanel->setPosition(glm::vec2(sidePos.x, tlPos.y));

        m_bottomPanel->setSize(glm::vec2(windowSize.x, windowSize.y - tlSize.y - sideSize.y - 2.f));
        glm::vec2 bottomSize{ m_bottomPanel->getSize() };
        m_bottomPanel->setPosition(glm::vec2(-1.f, windowSize.y - bottomSize.y));

        glm::ivec2 viewportSize{ (int)windowSize.x - sideSize.x,
            (int)windowSize.y - tlSize.y - bottomSize.y };
        Camera *camera{ engine->getCamera() };
        camera->setPosition({ (viewportSize.x - windowSize.x )/ 4.f, -1.f });
    }

    // Update emitter with UI values.
    Emitter *emitter{ engine->getEmitter() };
    if (emitter == nullptr)
        return;

    // Set colour.
    float red{ m_rSlider->getValue() / 255.f };
    float green{ m_gSlider->getValue() / 255.f };
    float blue{ m_bSlider->getValue() / 255.f };
    emitter->setColour(glm::vec3(red, green, blue));

    // Set position.
    glm::vec2 pos;
    bool isPosX{ m_xField->getValue(pos.x) };
    bool isPosY{ m_yField->getValue(pos.y) };
    if (isPosX || isPosY)
        emitter->setPosition(pos);

    // Set number of particles to generate.
    int numToGen;
    if (m_numGenField->getValue(numToGen))
        emitter->setNumToGenerate(numToGen);

    // Set spawn time.
    float spawnTime;
    if (m_spawnTimeField->getValue(spawnTime))
        emitter->setTimeToSpawn(spawnTime);

    // Set velocity.
    glm::vec2 velMin;
    bool isVelXMin{ m_velXMinField->getValue(velMin.x) };
    bool isVelYMin{ m_velYMinField->getValue(velMin.y) };
    if (isVelXMin || isVelYMin)
        emitter->setVelocityMin(velMin);

    // Convert max to offset.
    glm::vec2 velMax;
    bool isVelXMax{ m_velXMaxField->getValue(velMax.x) };
    bool isVelYMax{ m_velYMaxField->getValue(velMax.y) };
    if (isVelXMin || isVelYMin || isVelXMax || isVelYMax)
    {
        glm::vec2 velOffset{ velMax.x - velMin.x, velMax.y - velMin.y };
        velOffset = glm::clamp(velOffset, { 0.f, 0.f }, velOffset);
        emitter->setVelocityOffset(velOffset);
    }

    // Set acceleration.
    glm::vec2 accel;
    bool isAccelX{ m_accelXField->getValue(accel.x) };
    bool isAccelY{ m_accelYField->getValue(accel.y) };
    if (isAccelX || isAccelY)
        emitter->setAcceleration(accel);

    // Set particle size.
    float size;
    if (m_sizeField->getValue(size))
        emitter->setSize(size);

    // Set duration.
    float durationMin;
    bool isDurationMin{ m_durationMinField->getValue(durationMin) };
    if (isDurationMin)
        emitter->setDurationMin(durationMin);

    // Convert max to offset.
    float durationMax;
    bool isDurationMax{ m_durationMaxField->getValue(durationMax) };
    if (isDurationMin || isDurationMax)
    {
        float durationOffset{ durationMax - durationMin };
        durationOffset = glm::clamp(durationOffset, 0.f, durationOffset);
        emitter->setDurationOffset(durationOffset);
    }
}

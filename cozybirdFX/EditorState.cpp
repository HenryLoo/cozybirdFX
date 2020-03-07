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
    // Initialize top-left panel.
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

    // Initialize top-right panel.
    m_topRightPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, -1.f));

    m_particlesPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));
    m_visualsPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f));
    m_movementPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f));
    auto particles{ m_particlesPanel };
    auto visuals{ m_visualsPanel };
    auto movement{ m_movementPanel };

    auto particlesButton{ std::make_shared<UIButton>("Particles",
        [particles, visuals, movement]()
        {
            particles->setEnabled(true);
            visuals->setEnabled(false);
            movement->setEnabled(false);
        },
        buttonSize) };
    m_topRightPanel->addElement(particlesButton);

    auto visualsButton{ std::make_shared<UIButton>("Visuals",
        [particles, visuals, movement]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(true);
            movement->setEnabled(false);
        },
        buttonSize) };
    m_topRightPanel->addElement(visualsButton);

    auto movementButton{ std::make_shared<UIButton>("Movement",
        [particles, visuals, movement]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(false);
            movement->setEnabled(true);
        },
        buttonSize) };
    m_topRightPanel->addElement(movementButton);

    m_topRightPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_topRightPanel);

    // Initialize particles panel.
    m_particlesPanel->addNewLine();
    auto posLabel{ std::make_shared<UIText>("Position", glm::vec2(200, 16.f)) };
    m_particlesPanel->addElement(posLabel);

    m_particlesPanel->addNewHalfLine();
    m_xField = std::make_shared<UITextField>("x", glm::vec2(170.f, 24.f));
    m_xField->setValue(0.f);
    m_particlesPanel->addElement(m_xField);

    m_yField = std::make_shared<UITextField>("y", glm::vec2(170.f, 24.f));
    m_yField->setValue(0.f);
    m_particlesPanel->addElement(m_yField);

    m_particlesPanel->addNewLine();
    m_numGenField = std::make_shared<UITextField>("Particle Density",
        glm::vec2(356.f, 24.f));
    m_numGenField->setValue(30);
    m_particlesPanel->addElement(m_numGenField);

    m_particlesPanel->addNewLine();
    m_spawnTimeField = std::make_shared<UITextField>("Time to Spawn (s)",
        glm::vec2(356.f, 24.f));
    m_spawnTimeField->setValue(0.5f);
    m_particlesPanel->addElement(m_spawnTimeField);

    m_particlesPanel->addNewLine();
    m_sizeField = std::make_shared<UITextField>("Particle Size",
        glm::vec2(356.f, 24.f));
    m_sizeField->setValue(32.f);
    m_particlesPanel->addElement(m_sizeField);

    m_particlesPanel->addNewLine();
    auto durationLabel{ std::make_shared<UIText>("Duration", glm::vec2(200, 16.f)) };
    m_particlesPanel->addElement(durationLabel);

    m_particlesPanel->addNewHalfLine();
    m_durationMinField = std::make_shared<UITextField>("Min",
        glm::vec2(170.f, 24.f));
    m_durationMinField->setValue(2.f);
    m_particlesPanel->addElement(m_durationMinField);

    m_durationMaxField = std::make_shared<UITextField>("Max",
        glm::vec2(170.f, 24.f));
    m_durationMaxField->setValue(2.3f);
    m_particlesPanel->addElement(m_durationMaxField);

    m_particlesPanel->addNewLine();
    auto velXLabel{ std::make_shared<UIText>("Velocity.x", glm::vec2(200, 16.f)) };
    m_particlesPanel->addElement(velXLabel);

    m_particlesPanel->addNewHalfLine();
    m_velXMinField = std::make_shared<UITextField>("Min", glm::vec2(170.f, 24.f));
    m_velXMinField->setValue(-32.f);
    m_particlesPanel->addElement(m_velXMinField);

    m_velXMaxField = std::make_shared<UITextField>("Max", glm::vec2(170.f, 24.f));
    m_velXMaxField->setValue(32.f);
    m_particlesPanel->addElement(m_velXMaxField);

    m_particlesPanel->addNewLine();
    auto velYLabel{ std::make_shared<UIText>("Velocity.y", glm::vec2(200, 16.f)) };
    m_particlesPanel->addElement(velYLabel);

    m_particlesPanel->addNewHalfLine();
    m_velYMinField = std::make_shared<UITextField>("Min", glm::vec2(170.f, 24.f));
    m_velYMinField->setValue(-32.f);
    m_particlesPanel->addElement(m_velYMinField);

    m_velYMaxField = std::make_shared<UITextField>("Max", glm::vec2(170.f, 24.f));
    m_velYMaxField->setValue(32.f);
    m_particlesPanel->addElement(m_velYMaxField);

    m_particlesPanel->addNewLine();
    auto accelLabel{ std::make_shared<UIText>("Acceleration", glm::vec2(200, 16.f)) };
    m_particlesPanel->addElement(accelLabel);

    m_particlesPanel->addNewHalfLine();
    m_accelXField = std::make_shared<UITextField>("x", glm::vec2(170.f, 24.f));
    m_accelXField->setValue(0.f);
    m_particlesPanel->addElement(m_accelXField);

    m_accelYField = std::make_shared<UITextField>("y", glm::vec2(170.f, 24.f));
    m_accelYField->setValue(0.f);
    m_particlesPanel->addElement(m_accelYField);

    m_particlesPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_particlesPanel);

    // Initialize visuals panel.
    auto colourLabel{ std::make_shared<UIText>("Colour", glm::vec2(200, 16.f)) };
    m_visualsPanel->addElement(colourLabel);

    m_visualsPanel->addNewHalfLine();
    m_rSlider = std::make_shared<UISlider>("R", glm::vec2(0, 255),
        glm::vec2(356.f, 24.f));
    m_rSlider->setValue(50);
    m_visualsPanel->addElement(m_rSlider);

    m_visualsPanel->addNewHalfLine();
    m_gSlider = std::make_shared<UISlider>("G", glm::vec2(0, 255),
        glm::vec2(356.f, 24.f));
    m_gSlider->setValue(50);
    m_visualsPanel->addElement(m_gSlider);

    m_visualsPanel->addNewHalfLine();
    m_bSlider = std::make_shared<UISlider>("B", glm::vec2(0, 255),
        glm::vec2(356.f, 24.f));
    m_bSlider->setValue(50);
    m_visualsPanel->addElement(m_bSlider);

    m_visualsPanel->addToRenderer(uRenderer, tRenderer);
    m_visualsPanel->setEnabled(false);
    m_uiElements.push_back(m_visualsPanel);

    // Initialize movement panel.
    m_movementPanel->addToRenderer(uRenderer, tRenderer);
    m_movementPanel->setEnabled(false);
    m_uiElements.push_back(m_movementPanel);

    // Initialize bottom panel.
    m_bottomPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f), 
        glm::vec2(0.f, 0.f));
    m_bottomPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_bottomPanel);
}

void EditorState::handleInput(InputManager *inputManager)
{
    for (const auto &element : m_uiElements)
    {
        element->process(inputManager);
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
        m_particlesPanel->setSize(glm::vec2(-1.f, windowSize.y - tlSize.y - trSize.y - 2.f));
        glm::vec2 sideSize{ m_particlesPanel->getSize() };
        m_particlesPanel->setPosition(glm::vec2(windowSize.x - sideSize.x, trSize.y + 1.f));

        glm::vec2 sidePos{ m_particlesPanel->getPosition() };
        m_visualsPanel->setSize(sideSize);
        m_visualsPanel->setPosition(sidePos);
        m_movementPanel->setSize(sideSize);
        m_movementPanel->setPosition(sidePos);

        m_topLeftPanel->setSize(glm::vec2(windowSize.x - sideSize.x - 1.f, -1.f));
        m_topRightPanel->setSize(glm::vec2(sideSize.x, -1.f));
        glm::vec2 tlPos{ m_topLeftPanel->getPosition() };
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

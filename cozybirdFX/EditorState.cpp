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

namespace
{
    const glm::vec2 ONE_VAL_SIZE{ 356.f, 24.f };
    const glm::vec2 TWO_VAL_SIZE{ 170.f, ONE_VAL_SIZE.y };
    const glm::vec2 LABEL_SIZE{ 200.f, 16.f };
    const glm::vec2 COLOUR_RANGE{ 0.f, 255.f };
    const glm::vec2 COLOUR_SIZE{ 105.f, ONE_VAL_SIZE.y };
    const glm::vec2 BUTTON_SIZE{ 100.f, 32.f };
    const glm::vec2 ONE_BUTTON_SIZE{ ONE_VAL_SIZE.x, BUTTON_SIZE.y };
    const glm::vec2 TWO_BUTTON_SIZE{ TWO_VAL_SIZE.x, BUTTON_SIZE.y };
}

EditorState::EditorState(Engine *engine, TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    // Initialize top-left panel.
    m_topLeftPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, -1.f));

    auto fileButton{ std::make_shared<UIButton>("File", 
        []()
        { 
            std::cout << "File" << std::endl;
        },
        BUTTON_SIZE) };
    m_topLeftPanel->addElement(fileButton);

    auto exportButton{ std::make_shared<UIButton>("Export",
        [engine]()
        { 
            std::cout << "Export" << std::endl;
            engine->exportSpriteSheet();
        },
        BUTTON_SIZE) };
    m_topLeftPanel->addElement(exportButton);

    auto playButton{ std::make_shared<UIButton>("Play",
        []() { std::cout << "Play" << std::endl; },
        BUTTON_SIZE) };
    m_topLeftPanel->addElement(playButton);

    m_topLeftPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_topLeftPanel);

    // Initialize top-right panel.
    m_topRightPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, -1.f));

    m_particlesPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));
    m_visualsPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f));
    m_movementPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f));
    m_emittersPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f));
    m_renderPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f));
    auto particles{ m_particlesPanel };
    auto visuals{ m_visualsPanel };
    auto movement{ m_movementPanel };
    auto emitters{ m_emittersPanel };
    auto render{ m_renderPanel };

    auto particlesButton{ std::make_shared<UIButton>("Particles",
        [particles, visuals, movement, emitters, render]()
        {
            particles->setEnabled(true);
            visuals->setEnabled(false);
            movement->setEnabled(false);
            emitters->setEnabled(false);
            render->setEnabled(false);
        },
        BUTTON_SIZE) };
    m_topRightPanel->addElement(particlesButton);

    auto visualsButton{ std::make_shared<UIButton>("Visuals",
        [particles, visuals, movement, emitters, render]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(true);
            movement->setEnabled(false);
            emitters->setEnabled(false);
            render->setEnabled(false);
        },
        BUTTON_SIZE) };
    m_topRightPanel->addElement(visualsButton);

    auto movementButton{ std::make_shared<UIButton>("Movement",
        [particles, visuals, movement, emitters, render]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(false);
            movement->setEnabled(true);
            emitters->setEnabled(false);
            render->setEnabled(false);
        },
        BUTTON_SIZE) };
    m_topRightPanel->addElement(movementButton);

    auto emittersButton{ std::make_shared<UIButton>("Emitters",
        [particles, visuals, movement, emitters, render]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(false);
            movement->setEnabled(false);
            emitters->setEnabled(true);
            render->setEnabled(false);
        },
        BUTTON_SIZE) };
    m_topRightPanel->addElement(emittersButton);

    auto renderButton{ std::make_shared<UIButton>("Render",
        [particles, visuals, movement, emitters, render]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(false);
            movement->setEnabled(false);
            emitters->setEnabled(false);
            render->setEnabled(true);
        },
        BUTTON_SIZE) };
    m_topRightPanel->addElement(renderButton);

    m_topRightPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_topRightPanel);

    // Initialize particles panel.
    m_particlesPanel->addNewLine();
    auto posLabel{ std::make_shared<UIText>("Position", LABEL_SIZE) };
    m_particlesPanel->addElement(posLabel);

    m_particlesPanel->addNewHalfLine();
    m_xField = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_xField->setValue(0.f);
    m_particlesPanel->addElement(m_xField);

    m_yField = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_yField->setValue(0.f);
    m_particlesPanel->addElement(m_yField);

    m_particlesPanel->addNewLine();
    m_numGenField = std::make_shared<UITextField>("Particle Density", ONE_VAL_SIZE);
    m_numGenField->setValue(30);
    m_particlesPanel->addElement(m_numGenField);

    m_particlesPanel->addNewLine();
    m_spawnTimeField = std::make_shared<UITextField>("Time to Spawn", ONE_VAL_SIZE);
    m_spawnTimeField->setValue(0.5f);
    m_particlesPanel->addElement(m_spawnTimeField);

    m_particlesPanel->addNewLine();
    m_sizeField = std::make_shared<UITextField>("Particle Size", ONE_VAL_SIZE);
    m_sizeField->setValue(32.f);
    m_particlesPanel->addElement(m_sizeField);

    m_particlesPanel->addNewLine();
    m_delayField = std::make_shared<UITextField>("Delay Before Start", ONE_VAL_SIZE);
    m_delayField->setValue(0.f);
    m_particlesPanel->addElement(m_delayField);

    m_particlesPanel->addNewLine();
    auto durationLabel{ std::make_shared<UIText>("Duration", LABEL_SIZE) };
    m_particlesPanel->addElement(durationLabel);

    m_particlesPanel->addNewHalfLine();
    m_durationMinField = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_durationMinField->setValue(2.f);
    m_particlesPanel->addElement(m_durationMinField);

    m_durationMaxField = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_durationMaxField->setValue(2.3f);
    m_particlesPanel->addElement(m_durationMaxField);

    m_particlesPanel->addNewLine();
    auto velXLabel{ std::make_shared<UIText>("Velocity.x", LABEL_SIZE) };
    m_particlesPanel->addElement(velXLabel);

    m_particlesPanel->addNewHalfLine();
    m_velXMinField = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_velXMinField->setValue(-32.f);
    m_particlesPanel->addElement(m_velXMinField);

    m_velXMaxField = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_velXMaxField->setValue(32.f);
    m_particlesPanel->addElement(m_velXMaxField);

    m_particlesPanel->addNewLine();
    auto velYLabel{ std::make_shared<UIText>("Velocity.y", LABEL_SIZE) };
    m_particlesPanel->addElement(velYLabel);

    m_particlesPanel->addNewHalfLine();
    m_velYMinField = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_velYMinField->setValue(-32.f);
    m_particlesPanel->addElement(m_velYMinField);

    m_velYMaxField = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_velYMaxField->setValue(32.f);
    m_particlesPanel->addElement(m_velYMaxField);

    m_particlesPanel->addNewLine();
    auto accelLabel{ std::make_shared<UIText>("Acceleration", LABEL_SIZE) };
    m_particlesPanel->addElement(accelLabel);

    m_particlesPanel->addNewHalfLine();
    m_accelXField = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_accelXField->setValue(0.f);
    m_particlesPanel->addElement(m_accelXField);

    m_accelYField = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_accelYField->setValue(0.f);
    m_particlesPanel->addElement(m_accelYField);

    m_particlesPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_particlesPanel);

    // Initialize visuals panel.
    auto colourLabel{ std::make_shared<UIText>("Colour", LABEL_SIZE) };
    m_visualsPanel->addElement(colourLabel);

    m_visualsPanel->addNewHalfLine();
    m_rSlider = std::make_shared<UISlider>("R", COLOUR_RANGE, COLOUR_SIZE);
    m_rSlider->setValue(50);
    m_visualsPanel->addElement(m_rSlider);

    m_gSlider = std::make_shared<UISlider>("G", COLOUR_RANGE, COLOUR_SIZE);
    m_gSlider->setValue(50);
    m_visualsPanel->addElement(m_gSlider);

    m_bSlider = std::make_shared<UISlider>("B", COLOUR_RANGE, COLOUR_SIZE);
    m_bSlider->setValue(50);
    m_visualsPanel->addElement(m_bSlider);

    m_visualsPanel->addNewLine();
    auto birthColourLabel{ std::make_shared<UIText>("Birth Colour", LABEL_SIZE) };
    m_visualsPanel->addElement(birthColourLabel);

    m_visualsPanel->addNewHalfLine();
    m_birthRSlider = std::make_shared<UISlider>("R", COLOUR_RANGE, COLOUR_SIZE);
    m_birthRSlider->setValue(50);
    m_visualsPanel->addElement(m_birthRSlider);

    m_birthGSlider = std::make_shared<UISlider>("G", COLOUR_RANGE, COLOUR_SIZE);
    m_birthGSlider->setValue(50);
    m_visualsPanel->addElement(m_birthGSlider);

    m_birthBSlider = std::make_shared<UISlider>("B", COLOUR_RANGE, COLOUR_SIZE);
    m_birthBSlider->setValue(50);
    m_visualsPanel->addElement(m_birthBSlider);

    m_visualsPanel->addNewLine();
    auto deathColourLabel{ std::make_shared<UIText>("Death Colour", LABEL_SIZE) };
    m_visualsPanel->addElement(deathColourLabel);

    m_visualsPanel->addNewHalfLine();
    m_deathRSlider = std::make_shared<UISlider>("R", COLOUR_RANGE, COLOUR_SIZE);
    m_deathRSlider->setValue(50);
    m_visualsPanel->addElement(m_deathRSlider);

    m_deathGSlider = std::make_shared<UISlider>("G", COLOUR_RANGE, COLOUR_SIZE);
    m_deathGSlider->setValue(50);
    m_visualsPanel->addElement(m_deathGSlider);

    m_deathBSlider = std::make_shared<UISlider>("B", COLOUR_RANGE, COLOUR_SIZE);
    m_deathBSlider->setValue(50);
    m_visualsPanel->addElement(m_deathBSlider);

    m_visualsPanel->addNewLine();
    auto textureLabel{ std::make_shared<UIText>("Particle Texture", LABEL_SIZE) };
    m_visualsPanel->addElement(textureLabel);

    m_visualsPanel->addNewHalfLine();
    auto textureButton{ std::make_shared<UIButton>("Select...",
       []() {},
       ONE_BUTTON_SIZE) };
    m_visualsPanel->addElement(textureButton);

    m_visualsPanel->addToRenderer(uRenderer, tRenderer);
    m_visualsPanel->setEnabled(false);
    m_uiElements.push_back(m_visualsPanel);

    // Initialize movement panel.
    auto pathLabel{ std::make_shared<UIText>("Particle Path", LABEL_SIZE) };
    m_movementPanel->addElement(pathLabel);

    m_movementPanel->addNewHalfLine();
    auto leftPtLabel{ std::make_shared<UIText>("> Left click to move spline point.", LABEL_SIZE) };
    m_movementPanel->addElement(leftPtLabel);

    m_movementPanel->addNewHalfLine();
    auto rightPtLabel{ std::make_shared<UIText>("> Right click to add spline point.", LABEL_SIZE) };
    m_movementPanel->addElement(rightPtLabel);

    m_movementPanel->addNewHalfLine();
    m_particleSpeedField = std::make_shared<UITextField>("Particle Speed", ONE_VAL_SIZE);
    m_particleSpeedField->setValue(0.f);
    m_movementPanel->addElement(m_particleSpeedField);

    m_movementPanel->addNewLine();
    auto deletePathButton{ std::make_shared<UIButton>("Delete Path",
       []() { },
       ONE_BUTTON_SIZE) };
    m_movementPanel->addElement(deletePathButton);

    m_movementPanel->addNewLine();
    auto oscillateLabel{ std::make_shared<UIText>("Emitter Oscillation", LABEL_SIZE) };
    m_movementPanel->addElement(oscillateLabel);

    m_movementPanel->addNewLine();
    auto hSinLabel{ std::make_shared<UIText>("Horizontal Sine", LABEL_SIZE) };
    m_movementPanel->addElement(hSinLabel);

    m_movementPanel->addNewHalfLine();
    m_hSinAmountField = std::make_shared<UITextField>("Amount", TWO_VAL_SIZE);
    m_hSinAmountField->setValue(0.f);
    m_movementPanel->addElement(m_hSinAmountField);

    m_hSinPeriodField = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_hSinPeriodField->setValue(0.f);
    m_movementPanel->addElement(m_hSinPeriodField);

    m_movementPanel->addNewLine();
    auto vSinLabel{ std::make_shared<UIText>("Vertical Sine", LABEL_SIZE) };
    m_movementPanel->addElement(vSinLabel);

    m_movementPanel->addNewHalfLine();
    m_vSinAmountField = std::make_shared<UITextField>("Amount", TWO_VAL_SIZE);
    m_vSinAmountField->setValue(0.f);
    m_movementPanel->addElement(m_vSinAmountField);

    m_vSinPeriodField = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_vSinPeriodField->setValue(0.f);
    m_movementPanel->addElement(m_vSinPeriodField);

    m_movementPanel->addNewLine();
    auto circleLabel{ std::make_shared<UIText>("Circle", LABEL_SIZE) };
    m_movementPanel->addElement(circleLabel);

    m_movementPanel->addNewHalfLine();
    m_circleAmountField = std::make_shared<UITextField>("Amount", TWO_VAL_SIZE);
    m_circleAmountField->setValue(0.f);
    m_movementPanel->addElement(m_circleAmountField);

    m_circlePeriodField = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_circlePeriodField->setValue(0.f);
    m_movementPanel->addElement(m_circlePeriodField);

    m_movementPanel->addToRenderer(uRenderer, tRenderer);
    m_movementPanel->setEnabled(false);
    m_uiElements.push_back(m_movementPanel);

    // Initialize emitters panel.
    auto emittersLabel{ std::make_shared<UIText>("Select/Toggle Emitters", LABEL_SIZE) };
    m_emittersPanel->addElement(emittersLabel);

    m_emittersPanel->addNewLine();
    auto em1SelectButton{ std::make_shared<UIButton>("Emitter 1",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em1SelectButton);

    auto em1ToggleButton{ std::make_shared<UIButton>("ON",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em1ToggleButton);

    m_emittersPanel->addNewLine();
    auto em2SelectButton{ std::make_shared<UIButton>("Emitter 2",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em2SelectButton);

    auto em2ToggleButton{ std::make_shared<UIButton>("OFF",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em2ToggleButton);

    m_emittersPanel->addNewLine();
    auto em3SelectButton{ std::make_shared<UIButton>("Emitter 3",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em3SelectButton);

    auto em3ToggleButton{ std::make_shared<UIButton>("OFF",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em3ToggleButton);

    m_emittersPanel->addNewLine();
    auto em4SelectButton{ std::make_shared<UIButton>("Emitter 4",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em4SelectButton);

    auto em4ToggleButton{ std::make_shared<UIButton>("OFF",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em4ToggleButton);

    m_emittersPanel->addNewLine();
    auto em5SelectButton{ std::make_shared<UIButton>("Emitter 5",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em5SelectButton);

    auto em5ToggleButton{ std::make_shared<UIButton>("OFF",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em5ToggleButton);

    m_emittersPanel->addNewLine();
    auto em6SelectButton{ std::make_shared<UIButton>("Emitter 6",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em6SelectButton);

    auto em6ToggleButton{ std::make_shared<UIButton>("OFF",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em6ToggleButton);

    m_emittersPanel->addNewLine();
    auto em7SelectButton{ std::make_shared<UIButton>("Emitter 7",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em7SelectButton);

    auto em7ToggleButton{ std::make_shared<UIButton>("OFF",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em7ToggleButton);

    m_emittersPanel->addNewLine();
    auto em8SelectButton{ std::make_shared<UIButton>("Emitter 8",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em8SelectButton);

    auto em8ToggleButton{ std::make_shared<UIButton>("OFF",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em8ToggleButton);

    m_emittersPanel->addNewLine();
    auto em9SelectButton{ std::make_shared<UIButton>("Emitter 9",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em9SelectButton);

    auto em9ToggleButton{ std::make_shared<UIButton>("OFF",
       []() {},
       TWO_BUTTON_SIZE) };
    m_emittersPanel->addElement(em9ToggleButton);

    m_emittersPanel->addToRenderer(uRenderer, tRenderer);
    m_emittersPanel->setEnabled(false);
    m_uiElements.push_back(m_emittersPanel);

    // Initialize render panel.
    auto clipLabel{ std::make_shared<UIText>("Clip Size", LABEL_SIZE) };
    m_renderPanel->addElement(clipLabel);

    m_renderPanel->addNewHalfLine();
    m_clipXField = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_clipXField->setValue(400);
    m_renderPanel->addElement(m_clipXField);

    m_clipYField = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_clipYField->setValue(400);
    m_renderPanel->addElement(m_clipYField);

    m_renderPanel->addNewLine();
    m_durationField = std::make_shared<UITextField>("Animation Duration", ONE_VAL_SIZE);
    m_durationField->setValue(3.f);
    m_renderPanel->addElement(m_durationField);

    m_renderPanel->addNewLine();
    auto loopButton{ std::make_shared<UIButton>("Animation is Looping",
       []() {},
       ONE_BUTTON_SIZE) };
    m_renderPanel->addElement(loopButton);

    m_renderPanel->addNewLine();
    m_fpsField = std::make_shared<UITextField>("Export FPS", ONE_VAL_SIZE);
    m_fpsField->setValue(24);
    m_renderPanel->addElement(m_fpsField);

    m_renderPanel->addToRenderer(uRenderer, tRenderer);
    m_renderPanel->setEnabled(false);
    m_uiElements.push_back(m_renderPanel);

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
        m_emittersPanel->setSize(sideSize);
        m_emittersPanel->setPosition(sidePos);
        m_renderPanel->setSize(sideSize);
        m_renderPanel->setPosition(sidePos);

        m_topLeftPanel->setSize(glm::vec2(windowSize.x - trSize.x - 1.f, -1.f));
        glm::vec2 tlPos{ m_topLeftPanel->getPosition() };
        m_topRightPanel->setPosition(glm::vec2(windowSize.x - trSize.x, tlPos.y));

        m_bottomPanel->setSize(glm::vec2(windowSize.x, windowSize.y - tlSize.y - sideSize.y - 2.f));
        glm::vec2 bottomSize{ m_bottomPanel->getSize() };
        m_bottomPanel->setPosition(glm::vec2(-1.f, windowSize.y - bottomSize.y));

        glm::ivec2 viewportSize{ (int)windowSize.x - sideSize.x,
            (int)windowSize.y - tlSize.y - bottomSize.y };
        Camera *camera{ engine->getCamera() };
        float zoom{ camera->getZoom() };
        camera->setPosition({ (viewportSize.x - windowSize.x) / (2.f * zoom), -1.f });
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

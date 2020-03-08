#include "EditorState.h"
#include "Camera.h"
#include "Emitter.h"
#include "EmitterRenderer.h"
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

EditorState::EditorState(Engine *engine, EmitterRenderer *eRenderer, 
    TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    // Initialize UI panels.
    initTopLeftPanel(engine, eRenderer, tRenderer, uRenderer);
    initTopRightPanel(tRenderer, uRenderer);
    initBottomPanel(tRenderer, uRenderer);
    initParticlesPanel(tRenderer, uRenderer);
    initVisualsPanel(tRenderer, uRenderer);
    initMovementPanel(tRenderer, uRenderer);
    initEmittersPanel(engine, tRenderer, uRenderer);
    initRenderPanel(tRenderer, uRenderer);

    // Initialize the UI with the first emitter.
    updateUIFromEmitter(engine, 0);
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
    Emitter *emitter{ engine->getEmitter(m_emitter) };
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
    {
        emitter->setPosition(pos);
    }

    // Set number of particles to generate.
    int numToGen;
    if (m_numGenField->getValue(numToGen))
    {
        emitter->setNumToGenerate(numToGen);
    }

    // Set spawn time.
    float spawnTime;
    if (m_spawnTimeField->getValue(spawnTime))
    {
        emitter->setTimeToSpawn(spawnTime);
    }

    // Set velocity.
    glm::vec2 velMin;
    bool isVelXMin{ m_velXMinField->getValue(velMin.x) };
    bool isVelYMin{ m_velYMinField->getValue(velMin.y) };
    if (isVelXMin || isVelYMin)
    {
        emitter->setVelocityMin(velMin);
    }

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
    {
        emitter->setAcceleration(accel);
    }

    // Set particle size.
    float size;
    if (m_sizeField->getValue(size))
    {
        emitter->setSize(size);
    }

    // Set duration.
    float durationMin;
    bool isDurationMin{ m_durationMinField->getValue(durationMin) };
    if (isDurationMin)
    {
        emitter->setDurationMin(durationMin);
    }

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

void EditorState::updateUIFromEmitter(Engine *engine, int index)
{
    Emitter *emitter{ engine->getEmitter(index) };

    glm::vec2 pos{ emitter->getPosition() };
    m_xField->setValue(pos.x);
    m_yField->setValue(pos.y);

    m_numGenField->setValue(emitter->getNumToGenerate());
    m_spawnTimeField->setValue(emitter->getTimeToSpawn());

    glm::vec2 velMin{ emitter->getVelocityMin() };
    glm::vec2 velOffset{ emitter->getVelocityOffset() };
    m_velXMinField->setValue(velMin.x);
    m_velXMaxField->setValue(velMin.x + velOffset.x);
    m_velYMinField->setValue(velMin.y);
    m_velYMaxField->setValue(velMin.y + velOffset.y);

    glm::vec2 accel{ emitter->getAcceleration() };
    m_accelXField->setValue(accel.x);
    m_accelYField->setValue(accel.y);

    m_sizeField->setValue(emitter->getSize());

    float durationMin{ emitter->getDurationMin() };
    m_durationMinField->setValue(durationMin);
    m_durationMaxField->setValue(durationMin + emitter->getDurationOffset());

    glm::vec3 colour{ emitter->getColour() };
    m_rSlider->setValue(static_cast<int>(colour.r * COLOUR_RANGE.y));
    m_gSlider->setValue(static_cast<int>(colour.g * COLOUR_RANGE.y));
    m_bSlider->setValue(static_cast<int>(colour.b * COLOUR_RANGE.y));
}

void EditorState::initTopLeftPanel(Engine *engine, EmitterRenderer *eRenderer, 
    TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    m_topLeftPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, -1.f));

    auto fileButton{ std::make_shared<UIButton>("File",
        BUTTON_SIZE, false,
        []()
        {
            std::cout << "File" << std::endl;
        }) };
    m_topLeftPanel->addElement(fileButton);

    glm::ivec2 windowSize{ engine->getWindowSize() };
    auto exportButton{ std::make_shared<UIButton>("Export",
        BUTTON_SIZE, false,
        [eRenderer, windowSize]()
        {
            std::cout << "Export" << std::endl;
            eRenderer->exportSpriteSheet(windowSize);
        }) };
    m_topLeftPanel->addElement(exportButton);

    auto playButton{ std::make_shared<UIButton>("Play",
        BUTTON_SIZE, false,
        []() { std::cout << "Play" << std::endl; }) };
    m_topLeftPanel->addElement(playButton);

    m_topLeftPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_topLeftPanel);
}

void EditorState::initTopRightPanel(TextRenderer *tRenderer, 
    UIRenderer *uRenderer)
{
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
        BUTTON_SIZE, true) };
    auto visualsButton{ std::make_shared<UIButton>("Visuals",
        BUTTON_SIZE, true) };
    auto movementButton{ std::make_shared<UIButton>("Movement",
        BUTTON_SIZE, true) };
    auto emittersButton{ std::make_shared<UIButton>("Emitters",
        BUTTON_SIZE, true) };
    auto renderButton{ std::make_shared<UIButton>("Render",
        BUTTON_SIZE, true) };

    particlesButton->setAction([particles, visuals, movement, emitters, render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles->setEnabled(true);
            visuals->setEnabled(false);
            movement->setEnabled(false);
            emitters->setEnabled(false);
            render->setEnabled(false);

            particlesButton->setToggled(true);
            visualsButton->setToggled(false);
            movementButton->setToggled(false);
            emittersButton->setToggled(false);
            renderButton->setToggled(false);
        });

    visualsButton->setAction([particles, visuals, movement, emitters, render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(true);
            movement->setEnabled(false);
            emitters->setEnabled(false);
            render->setEnabled(false);

            particlesButton->setToggled(false);
            visualsButton->setToggled(true);
            movementButton->setToggled(false);
            emittersButton->setToggled(false);
            renderButton->setToggled(false);
        });

    movementButton->setAction([particles, visuals, movement, emitters, render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(false);
            movement->setEnabled(true);
            emitters->setEnabled(false);
            render->setEnabled(false);

            particlesButton->setToggled(false);
            visualsButton->setToggled(false);
            movementButton->setToggled(true);
            emittersButton->setToggled(false);
            renderButton->setToggled(false);
        });

    emittersButton->setAction([particles, visuals, movement, emitters, render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(false);
            movement->setEnabled(false);
            emitters->setEnabled(true);
            render->setEnabled(false);

            particlesButton->setToggled(false);
            visualsButton->setToggled(false);
            movementButton->setToggled(false);
            emittersButton->setToggled(true);
            renderButton->setToggled(false);
        });

    renderButton->setAction([particles, visuals, movement, emitters, render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles->setEnabled(false);
            visuals->setEnabled(false);
            movement->setEnabled(false);
            emitters->setEnabled(false);
            render->setEnabled(true);

            particlesButton->setToggled(false);
            visualsButton->setToggled(false);
            movementButton->setToggled(false);
            emittersButton->setToggled(false);
            renderButton->setToggled(true);
        });

    m_topRightPanel->addElement(particlesButton);
    m_topRightPanel->addElement(visualsButton);
    m_topRightPanel->addElement(movementButton);;
    m_topRightPanel->addElement(emittersButton);
    m_topRightPanel->addElement(renderButton);

    m_topRightPanel->addToRenderer(uRenderer, tRenderer);
    particlesButton->setToggled(true);
    m_uiElements.push_back(m_topRightPanel);
}

void EditorState::initBottomPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    m_bottomPanel = std::make_shared<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f));
    m_bottomPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_bottomPanel);
}

void EditorState::initParticlesPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    auto posLabel{ std::make_shared<UIText>("Position", LABEL_SIZE) };
    m_particlesPanel->addElement(posLabel);

    m_particlesPanel->addNewHalfLine();
    m_xField = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_xField);

    m_yField = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_yField);

    m_particlesPanel->addNewLine();
    m_numGenField = std::make_shared<UITextField>("Particle Density", ONE_VAL_SIZE);
    m_particlesPanel->addElement(m_numGenField);

    m_particlesPanel->addNewLine();
    m_spawnTimeField = std::make_shared<UITextField>("Time to Spawn", ONE_VAL_SIZE);
    m_particlesPanel->addElement(m_spawnTimeField);

    m_particlesPanel->addNewLine();
    m_sizeField = std::make_shared<UITextField>("Particle Size", ONE_VAL_SIZE);
    m_particlesPanel->addElement(m_sizeField);

    m_particlesPanel->addNewLine();
    m_delayField = std::make_shared<UITextField>("Delay Before Start", ONE_VAL_SIZE);
    m_particlesPanel->addElement(m_delayField);

    m_particlesPanel->addNewLine();
    auto durationLabel{ std::make_shared<UIText>("Duration", LABEL_SIZE) };
    m_particlesPanel->addElement(durationLabel);

    m_particlesPanel->addNewHalfLine();
    m_durationMinField = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_durationMinField);

    m_durationMaxField = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_durationMaxField);

    m_particlesPanel->addNewLine();
    auto velXLabel{ std::make_shared<UIText>("Velocity.x", LABEL_SIZE) };
    m_particlesPanel->addElement(velXLabel);

    m_particlesPanel->addNewHalfLine();
    m_velXMinField = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_velXMinField);

    m_velXMaxField = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_velXMaxField);

    m_particlesPanel->addNewLine();
    auto velYLabel{ std::make_shared<UIText>("Velocity.y", LABEL_SIZE) };
    m_particlesPanel->addElement(velYLabel);

    m_particlesPanel->addNewHalfLine();
    m_velYMinField = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_velYMinField);

    m_velYMaxField = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_velYMaxField);

    m_particlesPanel->addNewLine();
    auto accelLabel{ std::make_shared<UIText>("Acceleration", LABEL_SIZE) };
    m_particlesPanel->addElement(accelLabel);

    m_particlesPanel->addNewHalfLine();
    m_accelXField = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_accelXField);

    m_accelYField = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_particlesPanel->addElement(m_accelYField);

    m_particlesPanel->addToRenderer(uRenderer, tRenderer);
    m_uiElements.push_back(m_particlesPanel);
}

void EditorState::initVisualsPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    auto colourLabel{ std::make_shared<UIText>("Colour", LABEL_SIZE) };
    m_visualsPanel->addElement(colourLabel);

    m_visualsPanel->addNewHalfLine();
    m_rSlider = std::make_shared<UISlider>("R", COLOUR_RANGE, COLOUR_SIZE);
    m_visualsPanel->addElement(m_rSlider);

    m_gSlider = std::make_shared<UISlider>("G", COLOUR_RANGE, COLOUR_SIZE);
    m_visualsPanel->addElement(m_gSlider);

    m_bSlider = std::make_shared<UISlider>("B", COLOUR_RANGE, COLOUR_SIZE);
    m_visualsPanel->addElement(m_bSlider);

    m_visualsPanel->addNewLine();
    auto birthColourLabel{ std::make_shared<UIText>("Birth Colour", LABEL_SIZE) };
    m_visualsPanel->addElement(birthColourLabel);

    m_visualsPanel->addNewHalfLine();
    m_birthRSlider = std::make_shared<UISlider>("R", COLOUR_RANGE, COLOUR_SIZE);
    m_visualsPanel->addElement(m_birthRSlider);

    m_birthGSlider = std::make_shared<UISlider>("G", COLOUR_RANGE, COLOUR_SIZE);
    m_visualsPanel->addElement(m_birthGSlider);

    m_birthBSlider = std::make_shared<UISlider>("B", COLOUR_RANGE, COLOUR_SIZE);
    m_visualsPanel->addElement(m_birthBSlider);

    m_visualsPanel->addNewLine();
    auto deathColourLabel{ std::make_shared<UIText>("Death Colour", LABEL_SIZE) };
    m_visualsPanel->addElement(deathColourLabel);

    m_visualsPanel->addNewHalfLine();
    m_deathRSlider = std::make_shared<UISlider>("R", COLOUR_RANGE, COLOUR_SIZE);
    m_visualsPanel->addElement(m_deathRSlider);

    m_deathGSlider = std::make_shared<UISlider>("G", COLOUR_RANGE, COLOUR_SIZE);
    m_visualsPanel->addElement(m_deathGSlider);

    m_deathBSlider = std::make_shared<UISlider>("B", COLOUR_RANGE, COLOUR_SIZE);
    m_visualsPanel->addElement(m_deathBSlider);

    m_visualsPanel->addNewLine();
    auto textureLabel{ std::make_shared<UIText>("Particle Texture", LABEL_SIZE) };
    m_visualsPanel->addElement(textureLabel);

    m_visualsPanel->addNewHalfLine();
    auto textureButton{ std::make_shared<UIButton>("Select...", ONE_BUTTON_SIZE) };
    m_visualsPanel->addElement(textureButton);

    m_visualsPanel->addToRenderer(uRenderer, tRenderer);
    m_visualsPanel->setEnabled(false);
    m_uiElements.push_back(m_visualsPanel);
}

void EditorState::initMovementPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
{
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
    m_movementPanel->addElement(m_particleSpeedField);

    m_movementPanel->addNewLine();
    auto deletePathButton{ std::make_shared<UIButton>("Delete Path", ONE_BUTTON_SIZE) };
    m_movementPanel->addElement(deletePathButton);

    m_movementPanel->addNewLine();
    auto oscillateLabel{ std::make_shared<UIText>("Emitter Oscillation", LABEL_SIZE) };
    m_movementPanel->addElement(oscillateLabel);

    m_movementPanel->addNewLine();
    auto hSinLabel{ std::make_shared<UIText>("Horizontal Sine", LABEL_SIZE) };
    m_movementPanel->addElement(hSinLabel);

    m_movementPanel->addNewHalfLine();
    m_hSinAmountField = std::make_shared<UITextField>("Amount", TWO_VAL_SIZE);
    m_movementPanel->addElement(m_hSinAmountField);

    m_hSinPeriodField = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_movementPanel->addElement(m_hSinPeriodField);

    m_movementPanel->addNewLine();
    auto vSinLabel{ std::make_shared<UIText>("Vertical Sine", LABEL_SIZE) };
    m_movementPanel->addElement(vSinLabel);

    m_movementPanel->addNewHalfLine();
    m_vSinAmountField = std::make_shared<UITextField>("Amount", TWO_VAL_SIZE);
    m_movementPanel->addElement(m_vSinAmountField);

    m_vSinPeriodField = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_movementPanel->addElement(m_vSinPeriodField);

    m_movementPanel->addNewLine();
    auto circleLabel{ std::make_shared<UIText>("Circle", LABEL_SIZE) };
    m_movementPanel->addElement(circleLabel);

    m_movementPanel->addNewHalfLine();
    m_circleAmountField = std::make_shared<UITextField>("Amount", TWO_VAL_SIZE);
    m_movementPanel->addElement(m_circleAmountField);

    m_circlePeriodField = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_movementPanel->addElement(m_circlePeriodField);

    m_movementPanel->addToRenderer(uRenderer, tRenderer);
    m_movementPanel->setEnabled(false);
    m_uiElements.push_back(m_movementPanel);
}

void EditorState::initEmittersPanel(Engine *engine, TextRenderer *tRenderer, 
    UIRenderer *uRenderer)
{
    auto emittersLabel{ std::make_shared<UIText>("Select/Toggle Emitters", LABEL_SIZE) };
    m_emittersPanel->addElement(emittersLabel);

    m_emittersPanel->addNewLine();
    std::vector<std::shared_ptr<UIButton>> buttons;
    for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
    {
        auto emSelectButton{ std::make_shared<UIButton>("Emitter " + std::to_string(i + 1),
            TWO_BUTTON_SIZE, true) };
        buttons.push_back(emSelectButton);
    }

    std::shared_ptr<UIButton> firstToggle{ nullptr };
    for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
    {
        auto &button{ buttons[i] };
        button->setAction([this, engine, i, buttons]()
            {
                m_emitter = i;
                updateUIFromEmitter(engine, i);

                for (int j = 0; j < buttons.size(); ++j)
                {
                    bool val{ j == i };
                    buttons[j]->setToggled(val);
                }
            });
        m_emittersPanel->addElement(button);

        auto emToggleButton{ std::make_shared<UIButton>("ON", TWO_BUTTON_SIZE, true) };
        emToggleButton->setAction([engine, emToggleButton, i]()
            {
                engine->toggleEmitter(i, emToggleButton->isToggled());
            });
        m_emittersPanel->addElement(emToggleButton);

        if (i == 0)
            firstToggle = emToggleButton;

        if (i < EmitterRenderer::NUM_EMITTERS - 1)
            m_emittersPanel->addNewLine();
    }

    m_emittersPanel->addToRenderer(uRenderer, tRenderer);
    buttons[0]->setToggled(true);
    firstToggle->setToggled(true);
    m_emittersPanel->setEnabled(false);
    m_uiElements.push_back(m_emittersPanel);
}

void EditorState::initRenderPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    auto clipLabel{ std::make_shared<UIText>("Clip Size", LABEL_SIZE) };
    m_renderPanel->addElement(clipLabel);

    m_renderPanel->addNewHalfLine();
    m_clipXField = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_renderPanel->addElement(m_clipXField);

    m_clipYField = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_renderPanel->addElement(m_clipYField);

    m_renderPanel->addNewLine();
    m_durationField = std::make_shared<UITextField>("Animation Duration", ONE_VAL_SIZE);
    m_renderPanel->addElement(m_durationField);

    m_renderPanel->addNewLine();
    auto loopButton{ std::make_shared<UIButton>("Animation is Looping", ONE_BUTTON_SIZE) };
    m_renderPanel->addElement(loopButton);

    m_renderPanel->addNewLine();
    m_fpsField = std::make_shared<UITextField>("Export FPS", ONE_VAL_SIZE);
    m_renderPanel->addElement(m_fpsField);

    m_renderPanel->addToRenderer(uRenderer, tRenderer);
    m_renderPanel->setEnabled(false);
    m_uiElements.push_back(m_renderPanel);
}

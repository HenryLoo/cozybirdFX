#include "EditorState.h"
#include "Camera.h"
#include "Emitter.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "Texture.h"

#include "TopLeftPanel.h"
#include "TopRightPanel.h"
#include "BottomPanel.h"
#include "ParticlesPanel.h"
#include "VisualsPanel.h"
#include "MovementPanel.h"
#include "EmittersPanel.h"
#include "RenderPanel.h"

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
    const glm::vec2 COLOUR_SIZE{ 75.f, ONE_VAL_SIZE.y };
    const glm::vec2 BUTTON_SIZE{ 100.f, 32.f };
    const glm::vec2 ONE_BUTTON_SIZE{ ONE_VAL_SIZE.x, BUTTON_SIZE.y };
    const glm::vec2 TWO_BUTTON_SIZE{ TWO_VAL_SIZE.x, BUTTON_SIZE.y };
}

EditorState::EditorState(Engine *engine, AssetLoader *assetLoader,
    std::shared_ptr<EmitterRenderer> eRenderer, 
    TextRenderer *tRenderer, UIRenderer *uRenderer) :
    m_eRenderer(eRenderer)
{
    Camera *camera{ engine->getCamera() };
    UIRenderer::Properties clip;
    clip.size = eRenderer->getClipSize();
    clip.hasBorder = true;
    clip.colour = glm::vec4(0.f);
    auto it{ uRenderer->addElement(clip) };
    m_clipSizeBox = &*it;

    // Initialize UI panels.
    m_topLeftPanel = std::make_shared<TopLeftPanel>(engine, eRenderer, 
        tRenderer, uRenderer, m_clipSizeBox);
    m_particlesPanel = std::make_shared<ParticlesPanel>(tRenderer, uRenderer);
    m_visualsPanel = std::make_shared<VisualsPanel>(tRenderer, uRenderer, assetLoader);
    m_movementPanel = std::make_shared<MovementPanel>(tRenderer, uRenderer);
    m_emittersPanel = std::make_shared<EmittersPanel>(this, engine, tRenderer, 
        uRenderer);
    m_renderPanel = std::make_shared<RenderPanel>(this, eRenderer, tRenderer, 
        uRenderer, m_clipSizeBox);
    m_topRightPanel = std::make_shared<TopRightPanel>(tRenderer, uRenderer,
        m_particlesPanel, m_visualsPanel, m_movementPanel, m_emittersPanel,
        m_renderPanel);
    m_bottomPanel = std::make_shared<BottomPanel>(tRenderer, uRenderer);

    m_panels.push_back(m_topLeftPanel);
    m_panels.push_back(m_topRightPanel);
    m_panels.push_back(m_bottomPanel);
    m_panels.push_back(m_particlesPanel);
    m_panels.push_back(m_visualsPanel);
    m_panels.push_back(m_movementPanel);
    m_panels.push_back(m_emittersPanel);
    m_panels.push_back(m_renderPanel);

    // Initialize the UI with the first emitter.
    selectEmitter(engine, m_emitter);
}

void EditorState::handleInput(InputManager *inputManager)
{
    for (auto &panel : m_panels)
        panel->handleInput(inputManager);
}

void EditorState::update(Engine *engine, float deltaTime)
{
    if (engine == nullptr)
        return;

    // If window size changed, rescale UI.
    glm::vec2 windowSize{ engine->getWindowSize() };
    if (m_windowSize != windowSize)
    {
        Camera *camera{ engine->getCamera() };
        resize(windowSize, camera);
    }

    // Update emitter with UI values.
    Emitter *emitter{ engine->getEmitter(m_emitter) };
    if (emitter == nullptr)
        return;

    // Update panels.
    for (auto &panel : m_panels)
        panel->update(emitter, deltaTime);
}

void EditorState::selectEmitter(Engine *engine, int index)
{
    m_emitter = index;
    Emitter *emitter{ engine->getEmitter(m_emitter) };

    // Update panels.
    for (auto &panel : m_panels)
        panel->updateUIFromEmitter(emitter);
}

void EditorState::resize(glm::vec2 windowSize, Camera *camera)
{
    m_windowSize = windowSize;

    // Rescale and reposition side panels.
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

    // Rescale and reposition top panels.
    m_topLeftPanel->setSize(glm::vec2(windowSize.x - trSize.x - 1.f, -1.f));
    glm::vec2 tlPos{ m_topLeftPanel->getPosition() };
    m_topRightPanel->setPosition(glm::vec2(windowSize.x - trSize.x, tlPos.y));

    // Rescale and reposition bottom panel.
    m_bottomPanel->setSize(glm::vec2(windowSize.x, windowSize.y - tlSize.y - sideSize.y - 2.f));
    glm::vec2 bottomSize{ m_bottomPanel->getSize() };
    m_bottomPanel->setPosition(glm::vec2(-1.f, windowSize.y - bottomSize.y));

    // Reposition the camera.
    glm::ivec2 viewportSize{ getViewportSize() };
    float zoom{ camera->getZoom() };
    glm::vec2 cameraPos{ (viewportSize.x - windowSize.x) / (2.f * zoom), -1.f };
    camera->setPosition(cameraPos);

    // Reposition the clip size box.
    updateClipBoxPos();
}

glm::ivec2 EditorState::getViewportSize() const
{
    glm::vec2 sideSize{ m_particlesPanel->getSize() };
    glm::vec2 tlSize{ m_topLeftPanel->getSize() };
    glm::vec2 bottomSize{ m_bottomPanel->getSize() };

    glm::ivec2 viewportSize{ (int)m_windowSize.x - sideSize.x,
            (int)m_windowSize.y - tlSize.y - bottomSize.y };
    return viewportSize;
}

void EditorState::updateClipBoxPos()
{
    glm::vec2 tlSize{ m_topLeftPanel->getSize() };
    glm::ivec2 viewportSize{ getViewportSize() };

    m_clipSizeBox->pos = glm::round(glm::vec2(
        (viewportSize.x - m_clipSizeBox->size.x) / 2.f,
        (viewportSize.y - m_clipSizeBox->size.y) / 2.f + tlSize.y));
}

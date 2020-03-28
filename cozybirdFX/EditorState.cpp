#include "EditorState.h"
#include "Camera.h"
#include "Emitter.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "TextRenderer.h"
#include "Texture.h"

#include "TopLeftPanel.h"
#include "TopRightPanel.h"
#include "BottomPanel.h"
#include "ParticlesPanel.h"
#include "VisualsPanel.h"
#include "MovementPanel.h"
#include "EmittersPanel.h"
#include "RendererPanel.h"

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

EditorState::EditorState(Engine &engine, AssetLoader &assetLoader)
{
    m_tRenderer = std::make_shared<TextRenderer>(assetLoader);
    m_uRenderer = std::make_shared<UIRenderer>(assetLoader);
    m_eRenderer = std::make_shared<EmitterRenderer>(assetLoader);

    // Define the clip size box.
    UIRenderer::Properties clip;
    clip.size = m_eRenderer->getClipSize();
    clip.hasBorder = true;
    clip.colour = glm::vec4(0.f);
    auto it{ m_uRenderer->addElement(clip) };
    m_clipSizeBox = &*it;

    // Define the emitter position anchors.
    for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
    {
        UIRenderer::Properties anchor;
        anchor.size = glm::vec2(8.f);
        anchor.hasBorder = true;
        anchor.colour = glm::vec4(0.f, 0.f, 1.f, 1.f);
        anchor.isEnabled = false;
        auto it{ m_uRenderer->addElement(anchor) };
        m_emitterAnchors.push_back(&*it);
    }

    // Initialize UI panels.
    m_topLeftPanel = std::make_shared<TopLeftPanel>(engine, assetLoader,
        m_eRenderer, *m_tRenderer, *m_uRenderer, *m_clipSizeBox);

    m_particlesPanel = std::make_shared<ParticlesPanel>(*this, *m_tRenderer,
        *m_uRenderer);
    m_particlesPanel->setDescription("Right-click to move the current emitter.");
    m_currentPanel = m_particlesPanel;

    m_visualsPanel = std::make_shared<VisualsPanel>(*m_tRenderer, *m_uRenderer,
        assetLoader);

    m_movementPanel = std::make_shared<MovementPanel>(*m_tRenderer, *m_uRenderer);

    m_emittersPanel = std::make_shared<EmittersPanel>(*this, m_eRenderer,
        *m_tRenderer, *m_uRenderer);

    m_renderPanel = std::make_shared<RendererPanel>(*this, m_eRenderer,
        *m_tRenderer, *m_uRenderer, *m_clipSizeBox);

    m_topRightPanel = std::make_shared<TopRightPanel>(*this, *m_tRenderer, *m_uRenderer,
        m_particlesPanel, m_visualsPanel, m_movementPanel, m_emittersPanel,
        m_renderPanel);

    m_bottomPanel = std::make_shared<BottomPanel>(*this, *m_tRenderer, *m_uRenderer,
        m_particlesPanel, m_visualsPanel, m_movementPanel, m_emittersPanel,
        m_renderPanel);

    m_panels.push_back(m_topLeftPanel);
    m_panels.push_back(m_topRightPanel);
    m_panels.push_back(m_bottomPanel);
    m_panels.push_back(m_particlesPanel);
    m_panels.push_back(m_visualsPanel);
    m_panels.push_back(m_movementPanel);
    m_panels.push_back(m_emittersPanel);
    m_panels.push_back(m_renderPanel);

    // Initialize the UI with the first emitter.
    selectEmitter(m_emitter);
}

void EditorState::handleInput(InputManager &inputManager)
{
    for (auto &panel : m_panels)
        panel->handleInput(inputManager);
}

void EditorState::render(float deltaTime, const Camera &camera)
{
    m_eRenderer->render(deltaTime, camera);
    m_uRenderer->render(deltaTime, camera);
    m_tRenderer->render(deltaTime, camera);
}

void EditorState::enter()
{
    m_eRenderer->setPlaying(true);
    selectEmitter(m_emitter);
    updateClipBoxPos();
}

void EditorState::selectEmitter(int index)
{
    m_emitter = index;
    const Emitter &emitter{ m_eRenderer->getEmitter(index) };

    // Enable the emitter's position anchor.
    for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
    {
        m_emitterAnchors[i]->isEnabled = i == index;
    }

    // Update panels.
    for (auto &panel : m_panels)
        panel->updateUIFromEmitter(emitter);
}

void EditorState::update(float deltaTime, const Camera &camera)
{
    // Update emitter with UI values.
    Emitter &emitter{ m_eRenderer->getEmitter(m_emitter) };

    // Update the position of the emitter's anchor.
    glm::mat4 invProj{ glm::inverse(camera.getUIProjection()) };
    glm::mat4 proj{ camera.getSceneProjection() };
    glm::mat4 view{ camera.getView() };
    glm::vec2 emPos{ emitter.getPosition() };
    glm::vec2 halfSize{ m_emitterAnchors[m_emitter]->size / 2.f };
    emPos.x -= halfSize.x;
    emPos.y += halfSize.y;
    glm::vec4 pos{ emPos.x, emPos.y, 0.f, 1.f };
    pos = invProj * proj * view * pos;
    m_emitterAnchors[m_emitter]->pos = pos;

    // Update panels.
    for (auto &panel : m_panels)
        panel->update(deltaTime, emitter);
}

void EditorState::resize(Camera &camera)
{
    // Rescale and reposition side panels.
    glm::vec2 tlSize{ m_topLeftPanel->getSize() };
    glm::vec2 trSize{ m_topRightPanel->getSize() };
    m_particlesPanel->setSize(glm::vec2(-1.f, m_windowSize.y - tlSize.y - trSize.y - 2.f));
    glm::vec2 sideSize{ m_particlesPanel->getSize() };
    m_particlesPanel->setPosition(glm::vec2(m_windowSize.x - sideSize.x, trSize.y + 1.f));

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
    m_topLeftPanel->setSize(glm::vec2(m_windowSize.x - trSize.x - 1.f, -1.f));
    glm::vec2 tlPos{ m_topLeftPanel->getPosition() };
    m_topRightPanel->setPosition(glm::vec2(m_windowSize.x - trSize.x, tlPos.y));

    // Rescale and reposition bottom panel.
    m_bottomPanel->setSize(glm::vec2(m_windowSize.x, 
        m_windowSize.y - tlSize.y - sideSize.y - 2.f));
    glm::vec2 bottomSize{ m_bottomPanel->getSize() };
    m_bottomPanel->setPosition(glm::vec2(-1.f, m_windowSize.y - bottomSize.y));

    // Reposition the camera.
    glm::ivec2 viewportSize{ getViewportSize() };
    float zoom{ camera.getZoom() };
    glm::vec2 cameraPos{ (viewportSize.x - m_windowSize.x) / (2.f * zoom), -1.f };
    camera.setPosition(cameraPos);

    // Reposition the clip size box.
    updateClipBoxPos();
}

glm::ivec2 EditorState::getViewportSize() const
{
    glm::vec2 sideSize{ m_particlesPanel->getSize() };
    glm::vec2 tlSize{ m_topLeftPanel->getSize() };
    glm::vec2 bottomSize{ m_bottomPanel->getSize() };

    glm::ivec2 viewportSize{ 
        (int)m_windowSize.x - sideSize.x,
        (int)m_windowSize.y - tlSize.y - bottomSize.y
    };
    return viewportSize;
}

void EditorState::updateClipBoxPos()
{
    glm::ivec2 clipSize{ m_eRenderer->getClipSize() };
    m_clipSizeBox->size = clipSize;

    glm::vec2 tlSize{ m_topLeftPanel->getSize() };
    glm::ivec2 viewportSize{ getViewportSize() };

    m_clipSizeBox->pos = glm::round(glm::vec2(
    (viewportSize.x - clipSize.x) / 2.f,
        (viewportSize.y - clipSize.y) / 2.f + tlSize.y));
}

glm::vec2 EditorState::screenToClip(glm::vec2 screenPos) const
{
    glm::vec2 viewportSize{ getViewportSize() };
    glm::vec2 tlSize{ m_topLeftPanel->getSize() };
    glm::vec2 pos{
        screenPos.x - viewportSize.x / 2.f,
        -screenPos.y + viewportSize.y / 2.f + tlSize.y
    };
    
    return pos;
}

void EditorState::setCurrentPanel(std::shared_ptr<IEditorPanel> panel)
{
    m_currentPanel->setEnabled(false);
    m_currentPanel = panel;
    m_currentPanel->setEnabled(true);
}

std::shared_ptr<IEditorPanel> EditorState::getCurrentPanel() const
{
    return m_currentPanel;
}

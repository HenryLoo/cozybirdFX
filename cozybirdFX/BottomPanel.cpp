#include "BottomPanel.h"
#include "EditorState.h"
#include "UIText.h"

namespace
{
    const glm::vec2 DESCRIPTION_SIZE{ -1.f, 32.f };
}

BottomPanel::BottomPanel(EditorState &state, TextRenderer &tRenderer, 
    UIRenderer &uRenderer,
    std::shared_ptr<ParticlesPanel> particles,
    std::shared_ptr<VisualsPanel> visuals,
    std::shared_ptr<MovementPanel> movement,
    std::shared_ptr<EmittersPanel> emitters,
    std::shared_ptr<RendererPanel> renderer) :
    m_state(state)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f));

    m_text = std::make_shared<UIText>("", DESCRIPTION_SIZE);
    m_panel->addElement(m_text);

    m_panel->addToRenderer(uRenderer, tRenderer);
}

void BottomPanel::handleInput(InputManager &inputManager)
{
    IEditorPanel::handleInput(inputManager);

    std::string description;
    m_state.getCurrentPanel()->getDescription(inputManager, description);
    m_text->setText(description);
}

void BottomPanel::update(float deltaTime, Emitter &emitter)
{

}

void BottomPanel::updateUIFromEmitter(const Emitter &emitter)
{
}
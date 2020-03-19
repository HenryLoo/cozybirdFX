#include "BottomPanel.h"

BottomPanel::BottomPanel(TextRenderer &tRenderer, UIRenderer &uRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f));
    m_panel->addToRenderer(uRenderer, tRenderer);
}

void BottomPanel::update(float deltaTime, Emitter &emitter)
{
}

void BottomPanel::updateUIFromEmitter(const Emitter &emitter)
{
}

#include "TopRightPanel.h"
#include "EditorState.h"
#include "UIButton.h"

#include "ParticlesPanel.h"
#include "VisualsPanel.h"
#include "MovementPanel.h"
#include "EmittersPanel.h"
#include "RendererPanel.h"
#include <iostream>
TopRightPanel::TopRightPanel(EditorState &editor, SpriteRenderer &sRenderer, 
    TextRenderer &tRenderer, UIRenderer &uRenderer,
    std::shared_ptr<ParticlesPanel> particles,
    std::shared_ptr<VisualsPanel> visuals,
    std::shared_ptr<MovementPanel> movement,
    std::shared_ptr<EmittersPanel> emitters,
    std::shared_ptr<RendererPanel> renderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, -1.f));

    m_particlesButton = std::make_shared<UIButton>("Particles",
        BUTTON_SIZE, true);
    m_visualsButton = std::make_shared<UIButton>("Visuals",
        BUTTON_SIZE, true);
    m_movementButton = std::make_shared<UIButton>("Movement",
        BUTTON_SIZE, true);
    m_emittersButton = std::make_shared<UIButton>("Emitters",
        BUTTON_SIZE, true);
    m_rendererButton = std::make_shared<UIButton>("Renderer",
        BUTTON_SIZE, true);

    m_particlesButton->setAction([&editor, particles, this]()
        {
            editor.setCurrentPanel(particles);
            setCurrentButton(m_particlesButton);
        });

    m_visualsButton->setAction([&editor, visuals, this]()
        {
            editor.setCurrentPanel(visuals);
            setCurrentButton(m_visualsButton);
        });

    m_movementButton->setAction([&editor, movement, this]()
        {
            editor.setCurrentPanel(movement);
            setCurrentButton(m_movementButton);
        });

    m_emittersButton->setAction([&editor, emitters, this]()
        {
            editor.setCurrentPanel(emitters);
            setCurrentButton(m_emittersButton);
        });

    m_rendererButton->setAction([&editor, renderer, this]()
        {
            editor.setCurrentPanel(renderer);
            setCurrentButton(m_rendererButton);
        });

    m_panel->addElement(m_particlesButton);
    m_panel->addElement(m_visualsButton);
    m_panel->addElement(m_movementButton);;
    m_panel->addElement(m_emittersButton);
    m_panel->addElement(m_rendererButton);

    m_panel->addToRenderer(sRenderer, uRenderer, tRenderer);
    m_particlesButton->setToggled(true);
}

void TopRightPanel::update(float deltaTime, Emitter &emitter)
{
}

void TopRightPanel::updateUIFromEmitter(const Emitter &emitter)
{
}

void TopRightPanel::setCurrentButton(std::shared_ptr<UIButton> button)
{
    m_currentButton = button;

    m_particlesButton->setToggled(m_particlesButton == m_currentButton);
    m_visualsButton->setToggled(m_visualsButton == m_currentButton);
    m_movementButton->setToggled(m_movementButton == m_currentButton);
    m_emittersButton->setToggled(m_emittersButton == m_currentButton);
    m_rendererButton->setToggled(m_rendererButton == m_currentButton);
}

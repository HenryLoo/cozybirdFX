#include "TopRightPanel.h"
#include "UIButton.h"

#include "ParticlesPanel.h"
#include "VisualsPanel.h"
#include "MovementPanel.h"
#include "EmittersPanel.h"
#include "RenderPanel.h"

TopRightPanel::TopRightPanel(TextRenderer &tRenderer, UIRenderer &uRenderer,
    ParticlesPanel &particles, VisualsPanel &visuals,
    MovementPanel &movement, EmittersPanel &emitters,
    RenderPanel &render)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, -1.f));

    auto particlesButton{ std::make_shared<UIButton>("Particles",
        BUTTON_SIZE, true) };
    auto visualsButton{ std::make_shared<UIButton>("Visuals",
        BUTTON_SIZE, true) };
    auto movementButton{ std::make_shared<UIButton>("Movement",
        BUTTON_SIZE, true) };
    auto emittersButton{ std::make_shared<UIButton>("Emitters",
        BUTTON_SIZE, true) };
    auto renderButton{ std::make_shared<UIButton>("Renderer",
        BUTTON_SIZE, true) };

    particlesButton->setAction([&particles, &visuals, &movement, &emitters, &render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles.setEnabled(true);
            visuals.setEnabled(false);
            movement.setEnabled(false);
            emitters.setEnabled(false);
            render.setEnabled(false);

            particlesButton->setToggled(true);
            visualsButton->setToggled(false);
            movementButton->setToggled(false);
            emittersButton->setToggled(false);
            renderButton->setToggled(false);
        });

    visualsButton->setAction([&particles, &visuals, &movement, &emitters, &render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles.setEnabled(false);
            visuals.setEnabled(true);
            movement.setEnabled(false);
            emitters.setEnabled(false);
            render.setEnabled(false);

            particlesButton->setToggled(false);
            visualsButton->setToggled(true);
            movementButton->setToggled(false);
            emittersButton->setToggled(false);
            renderButton->setToggled(false);
        });

    movementButton->setAction([&particles, &visuals, &movement, &emitters, &render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles.setEnabled(false);
            visuals.setEnabled(false);
            movement.setEnabled(true);
            emitters.setEnabled(false);
            render.setEnabled(false);

            particlesButton->setToggled(false);
            visualsButton->setToggled(false);
            movementButton->setToggled(true);
            emittersButton->setToggled(false);
            renderButton->setToggled(false);
        });

    emittersButton->setAction([&particles, &visuals, &movement, &emitters, &render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles.setEnabled(false);
            visuals.setEnabled(false);
            movement.setEnabled(false);
            emitters.setEnabled(true);
            render.setEnabled(false);

            particlesButton->setToggled(false);
            visualsButton->setToggled(false);
            movementButton->setToggled(false);
            emittersButton->setToggled(true);
            renderButton->setToggled(false);
        });

    renderButton->setAction([&particles, &visuals, &movement, &emitters, &render,
        particlesButton, visualsButton, movementButton, emittersButton, renderButton]()
        {
            particles.setEnabled(false);
            visuals.setEnabled(false);
            movement.setEnabled(false);
            emitters.setEnabled(false);
            render.setEnabled(true);

            particlesButton->setToggled(false);
            visualsButton->setToggled(false);
            movementButton->setToggled(false);
            emittersButton->setToggled(false);
            renderButton->setToggled(true);
        });

    m_panel->addElement(particlesButton);
    m_panel->addElement(visualsButton);
    m_panel->addElement(movementButton);;
    m_panel->addElement(emittersButton);
    m_panel->addElement(renderButton);

    m_panel->addToRenderer(uRenderer, tRenderer);
    particlesButton->setToggled(true);
}

void TopRightPanel::update(float deltaTime, Emitter &emitter)
{
}

void TopRightPanel::updateUIFromEmitter(const Emitter &emitter)
{
}

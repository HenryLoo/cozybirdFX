#include "VisualsPanel.h"
#include "Emitter.h"
#include "UIButton.h"
#include "UISlider.h"
#include "UIText.h"

VisualsPanel::VisualsPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    auto colourLabel{ std::make_shared<UIText>("Colour", LABEL_SIZE) };
    m_panel->addElement(colourLabel);

    m_panel->addNewHalfLine();
    m_rSlider = std::make_shared<UISlider>("R", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_rSlider);

    m_gSlider = std::make_shared<UISlider>("G", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_gSlider);

    m_bSlider = std::make_shared<UISlider>("B", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_bSlider);

    m_aSlider = std::make_shared<UISlider>("A", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_aSlider);

    m_panel->addNewLine();
    auto birthColourLabel{ std::make_shared<UIText>("Birth Colour", LABEL_SIZE) };
    m_panel->addElement(birthColourLabel);

    m_panel->addNewHalfLine();
    m_birthRSlider = std::make_shared<UISlider>("R", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_birthRSlider);

    m_birthGSlider = std::make_shared<UISlider>("G", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_birthGSlider);

    m_birthBSlider = std::make_shared<UISlider>("B", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_birthBSlider);

    m_birthASlider = std::make_shared<UISlider>("A", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_birthASlider);

    m_panel->addNewLine();
    auto deathColourLabel{ std::make_shared<UIText>("Death Colour", LABEL_SIZE) };
    m_panel->addElement(deathColourLabel);

    m_panel->addNewHalfLine();
    m_deathRSlider = std::make_shared<UISlider>("R", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_deathRSlider);

    m_deathGSlider = std::make_shared<UISlider>("G", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_deathGSlider);

    m_deathBSlider = std::make_shared<UISlider>("B", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_deathBSlider);

    m_deathASlider = std::make_shared<UISlider>("A", COLOUR_RANGE, COLOUR_SIZE);
    m_panel->addElement(m_deathASlider);

    m_panel->addNewLine();
    auto textureLabel{ std::make_shared<UIText>("Particle Texture", LABEL_SIZE) };
    m_panel->addElement(textureLabel);

    m_panel->addNewHalfLine();
    auto textureButton{ std::make_shared<UIButton>("Select...", ONE_BUTTON_SIZE) };
    m_panel->addElement(textureButton);

    m_panel->addToRenderer(uRenderer, tRenderer);
    m_panel->setEnabled(false);
}

void VisualsPanel::update(Emitter *emitter, float deltaTime)
{
    // Set colour.
    float red{ m_rSlider->getValue() / COLOUR_RANGE.y };
    float green{ m_gSlider->getValue() / COLOUR_RANGE.y };
    float blue{ m_bSlider->getValue() / COLOUR_RANGE.y };
    float alpha{ m_aSlider->getValue() / COLOUR_RANGE.y };
    emitter->setColour(glm::vec4(red, green, blue, alpha));

    // Set birth colour.
    float birthRed{ m_birthRSlider->getValue() / COLOUR_RANGE.y };
    float birthGreen{ m_birthGSlider->getValue() / COLOUR_RANGE.y };
    float birthBlue{ m_birthBSlider->getValue() / COLOUR_RANGE.y };
    float birthAlpha{ m_birthASlider->getValue() / COLOUR_RANGE.y };
    emitter->setBirthColour(glm::vec4(birthRed, birthGreen, birthBlue, birthAlpha));

    // Set death colour.
    float deathRed{ m_deathRSlider->getValue() / COLOUR_RANGE.y };
    float deathGreen{ m_deathGSlider->getValue() / COLOUR_RANGE.y };
    float deathBlue{ m_deathBSlider->getValue() / COLOUR_RANGE.y };
    float deathAlpha{ m_deathASlider->getValue() / COLOUR_RANGE.y };
    emitter->setDeathColour(glm::vec4(deathRed, deathGreen, deathBlue, deathAlpha));
}

void VisualsPanel::updateUIFromEmitter(Emitter *emitter)
{
    glm::vec4 colour{ emitter->getColour() };
    m_rSlider->setValue(static_cast<int>(colour.r * COLOUR_RANGE.y));
    m_gSlider->setValue(static_cast<int>(colour.g * COLOUR_RANGE.y));
    m_bSlider->setValue(static_cast<int>(colour.b * COLOUR_RANGE.y));
    m_aSlider->setValue(static_cast<int>(colour.a * COLOUR_RANGE.y));

    glm::vec4 birthColour{ emitter->getBirthColour() };
    m_birthRSlider->setValue(static_cast<int>(birthColour.r * COLOUR_RANGE.y));
    m_birthGSlider->setValue(static_cast<int>(birthColour.g * COLOUR_RANGE.y));
    m_birthBSlider->setValue(static_cast<int>(birthColour.b * COLOUR_RANGE.y));
    m_birthASlider->setValue(static_cast<int>(birthColour.a * COLOUR_RANGE.y));

    glm::vec4 deathColour{ emitter->getDeathColour() };
    m_deathRSlider->setValue(static_cast<int>(deathColour.r * COLOUR_RANGE.y));
    m_deathGSlider->setValue(static_cast<int>(deathColour.g * COLOUR_RANGE.y));
    m_deathBSlider->setValue(static_cast<int>(deathColour.b * COLOUR_RANGE.y));
    m_deathASlider->setValue(static_cast<int>(deathColour.a * COLOUR_RANGE.y));
}

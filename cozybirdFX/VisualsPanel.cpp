#include "VisualsPanel.h"
#include "Emitter.h"
#include "UIButton.h"
#include "UISlider.h"
#include "UIText.h"

namespace
{
    const glm::vec2 COLOUR_RANGE{ 0.f, 255.f };
    const glm::vec2 PERCENT_RANGE{ 0.f, 100.f };
}

VisualsPanel::VisualsPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    auto colourLabel{ std::make_shared<UIText>("Colour", LABEL_SIZE) };
    m_panel->addElement(colourLabel);

    m_panel->addNewHalfLine();
    m_red = std::make_shared<UISlider>("R", COLOUR_RANGE, THREE_VAL_SIZE);
    m_panel->addElement(m_red);

    m_green = std::make_shared<UISlider>("G", COLOUR_RANGE, THREE_VAL_SIZE);
    m_panel->addElement(m_green);

    m_blue = std::make_shared<UISlider>("B", COLOUR_RANGE, THREE_VAL_SIZE);
    m_panel->addElement(m_blue);

    m_panel->addNewHalfLine();
    m_opacity = std::make_shared<UISlider>("Opacity", PERCENT_RANGE, TWO_VAL_SIZE);
    m_panel->addElement(m_opacity);

    m_additivity = std::make_shared<UISlider>("Additivity", PERCENT_RANGE, TWO_VAL_SIZE);
    m_panel->addElement(m_additivity);

    m_panel->addNewLine();
    auto birthColourLabel{ std::make_shared<UIText>("Birth Colour", LABEL_SIZE) };
    m_panel->addElement(birthColourLabel);

    m_panel->addNewHalfLine();
    m_birthRed = std::make_shared<UISlider>("R", COLOUR_RANGE, THREE_VAL_SIZE);
    m_panel->addElement(m_birthRed);

    m_birthGreen = std::make_shared<UISlider>("G", COLOUR_RANGE, THREE_VAL_SIZE);
    m_panel->addElement(m_birthGreen);

    m_birthBlue = std::make_shared<UISlider>("B", COLOUR_RANGE, THREE_VAL_SIZE);
    m_panel->addElement(m_birthBlue);

    m_panel->addNewHalfLine();
    m_birthOpacity = std::make_shared<UISlider>("Opacity", PERCENT_RANGE, TWO_VAL_SIZE);
    m_panel->addElement(m_birthOpacity);

    m_birthAdditivity = std::make_shared<UISlider>("Additivity", PERCENT_RANGE, TWO_VAL_SIZE);
    m_panel->addElement(m_birthAdditivity);

    m_panel->addNewLine();
    auto deathColourLabel{ std::make_shared<UIText>("Death Colour", LABEL_SIZE) };
    m_panel->addElement(deathColourLabel);

    m_panel->addNewHalfLine();
    m_deathRed = std::make_shared<UISlider>("R", COLOUR_RANGE, THREE_VAL_SIZE);
    m_panel->addElement(m_deathRed);

    m_deathGreen = std::make_shared<UISlider>("G", COLOUR_RANGE, THREE_VAL_SIZE);
    m_panel->addElement(m_deathGreen);

    m_deathBlue = std::make_shared<UISlider>("B", COLOUR_RANGE, THREE_VAL_SIZE);
    m_panel->addElement(m_deathBlue);

    m_panel->addNewHalfLine();
    m_deathOpacity = std::make_shared<UISlider>("Opacity", PERCENT_RANGE, TWO_VAL_SIZE);
    m_panel->addElement(m_deathOpacity);

    m_deathAdditivity = std::make_shared<UISlider>("Additivity", PERCENT_RANGE, TWO_VAL_SIZE);
    m_panel->addElement(m_deathAdditivity);

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
    float red{ m_red->getValue() / COLOUR_RANGE.y };
    float green{ m_green->getValue() / COLOUR_RANGE.y };
    float blue{ m_blue->getValue() / COLOUR_RANGE.y };
    float alpha{ m_opacity->getValue() / PERCENT_RANGE.y };
    emitter->setColour(glm::vec4(red, green, blue, alpha));

    float additivity{ m_additivity->getValue() / PERCENT_RANGE.y };
    emitter->setAdditivity(additivity);

    // Set birth colour.
    float birthRed{ m_birthRed->getValue() / COLOUR_RANGE.y };
    float birthGreen{ m_birthGreen->getValue() / COLOUR_RANGE.y };
    float birthBlue{ m_birthBlue->getValue() / COLOUR_RANGE.y };
    float birthAlpha{ m_birthOpacity->getValue() / PERCENT_RANGE.y };
    emitter->setBirthColour(glm::vec4(birthRed, birthGreen, birthBlue, birthAlpha));

    float birthAdditivity{ m_birthAdditivity->getValue() / PERCENT_RANGE.y };
    emitter->setBirthAdditivity(birthAdditivity);

    // Set death colour.
    float deathRed{ m_deathRed->getValue() / COLOUR_RANGE.y };
    float deathGreen{ m_deathGreen->getValue() / COLOUR_RANGE.y };
    float deathBlue{ m_deathBlue->getValue() / COLOUR_RANGE.y };
    float deathAlpha{ m_deathOpacity->getValue() / PERCENT_RANGE.y };
    emitter->setDeathColour(glm::vec4(deathRed, deathGreen, deathBlue, deathAlpha));

    float deathAdditivity{ m_deathAdditivity->getValue() / PERCENT_RANGE.y };
    emitter->setDeathAdditivity(deathAdditivity);
}

void VisualsPanel::updateUIFromEmitter(Emitter *emitter)
{
    glm::vec4 colour{ emitter->getColour() };
    m_red->setValue(static_cast<int>(colour.r * COLOUR_RANGE.y));
    m_green->setValue(static_cast<int>(colour.g * COLOUR_RANGE.y));
    m_blue->setValue(static_cast<int>(colour.b * COLOUR_RANGE.y));
    m_opacity->setValue(static_cast<int>(colour.a * PERCENT_RANGE.y));
    m_additivity->setValue(static_cast<int>(emitter->getAdditivity() * PERCENT_RANGE.y));

    glm::vec4 birthColour{ emitter->getBirthColour() };
    m_birthRed->setValue(static_cast<int>(birthColour.r * COLOUR_RANGE.y));
    m_birthGreen->setValue(static_cast<int>(birthColour.g * COLOUR_RANGE.y));
    m_birthBlue->setValue(static_cast<int>(birthColour.b * COLOUR_RANGE.y));
    m_birthOpacity->setValue(static_cast<int>(birthColour.a * PERCENT_RANGE.y));
    m_birthAdditivity->setValue(static_cast<int>(emitter->getBirthAdditivity() * PERCENT_RANGE.y));

    glm::vec4 deathColour{ emitter->getDeathColour() };
    m_deathRed->setValue(static_cast<int>(deathColour.r * COLOUR_RANGE.y));
    m_deathGreen->setValue(static_cast<int>(deathColour.g * COLOUR_RANGE.y));
    m_deathBlue->setValue(static_cast<int>(deathColour.b * COLOUR_RANGE.y));
    m_deathOpacity->setValue(static_cast<int>(deathColour.a * PERCENT_RANGE.y));
    m_deathAdditivity->setValue(static_cast<int>(emitter->getDeathAdditivity() * PERCENT_RANGE.y));
}
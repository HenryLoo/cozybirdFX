#include "VisualsPanel.h"
#include "AssetLoader.h"
#include "Emitter.h"
#include "Texture.h"
#include "UIButton.h"
#include "UISlider.h"
#include "UIText.h"
#include "UIField.h"

namespace
{
    const glm::vec2 COLOUR_RANGE{ 0.f, 255.f };
    const glm::vec2 PERCENT_RANGE{ 0.f, 100.f };
}

VisualsPanel::VisualsPanel(TextRenderer &tRenderer, UIRenderer &uRenderer, 
    AssetLoader &assetLoader) :
    m_assetLoader(assetLoader)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    auto blendLabel{ std::make_shared<UIText>("Blend Mode", LABEL_SIZE) };
    m_panel->addElement(blendLabel);

    m_panel->addNewHalfLine();
    m_linearBlend = std::make_shared<UIButton>("Linear", TWO_BUTTON_SIZE, true);
    m_linearBlend->setDescription("Switch to linear blending for this emitter's particles.");
    m_additiveBlend = std::make_shared<UIButton>("Additive", TWO_BUTTON_SIZE, true);
    m_additiveBlend->setDescription("Switch to additive blending for this emitter's particles.");
    auto linearBlend{ m_linearBlend };
    auto additiveBlend{ m_additiveBlend };
    m_linearBlend->setAction([this, linearBlend, additiveBlend]()
        {
            linearBlend->setToggled(true);
            additiveBlend->setToggled(false);
        });
    m_panel->addElement(m_linearBlend);

    m_additiveBlend->setAction([this, linearBlend, additiveBlend]()
        {
            linearBlend->setToggled(false);
            additiveBlend->setToggled(true);
        });
    m_panel->addElement(m_additiveBlend);

    m_panel->addNewLine();
    auto birthColourLabel{ std::make_shared<UIText>("Birth Colour", LABEL_SIZE) };
    m_panel->addElement(birthColourLabel);

    m_panel->addNewHalfLine();
    m_birthRed = std::make_shared<UISlider>("R", COLOUR_RANGE, THREE_VAL_SIZE);
    m_birthRed->setDescription("Set the amount of red that a particle's colour will start with.");
    m_panel->addElement(m_birthRed);

    m_birthGreen = std::make_shared<UISlider>("G", COLOUR_RANGE, THREE_VAL_SIZE);
    m_birthGreen->setDescription("Set the amount of green that a particle's colour will start with.");
    m_panel->addElement(m_birthGreen);

    m_birthBlue = std::make_shared<UISlider>("B", COLOUR_RANGE, THREE_VAL_SIZE);
    m_birthBlue->setDescription("Set the amount of blue that a particle's colour will start with.");
    m_panel->addElement(m_birthBlue);

    m_panel->addNewHalfLine();
    m_birthOpacity = std::make_shared<UISlider>("Opacity", PERCENT_RANGE, ONE_VAL_SIZE);
    m_birthOpacity->setDescription("Set the level of transparency that a particle will start with.");
    m_panel->addElement(m_birthOpacity);

    m_panel->addNewLine();
    auto colourLabel{ std::make_shared<UIText>("Colour", LABEL_SIZE) };
    m_panel->addElement(colourLabel);

    m_panel->addNewHalfLine();
    m_red = std::make_shared<UISlider>("R", COLOUR_RANGE, THREE_VAL_SIZE);
    m_red->setDescription("Set the amount of red that a particle's colour will have during its life.");
    m_panel->addElement(m_red);

    m_green = std::make_shared<UISlider>("G", COLOUR_RANGE, THREE_VAL_SIZE);
    m_green->setDescription("Set the amount of green that a particle's colour will have during its life.");
    m_panel->addElement(m_green);

    m_blue = std::make_shared<UISlider>("B", COLOUR_RANGE, THREE_VAL_SIZE);
    m_blue->setDescription("Set the amount of blue that a particle's colour will have during its life.");
    m_panel->addElement(m_blue);

    m_panel->addNewHalfLine();
    m_opacity = std::make_shared<UISlider>("Opacity", PERCENT_RANGE, ONE_VAL_SIZE);
    m_opacity->setDescription("Set the level of transparency that a particle will have during its life.");
    m_panel->addElement(m_opacity);

    m_panel->addNewLine();
    auto deathColourLabel{ std::make_shared<UIText>("Death Colour", LABEL_SIZE) };
    m_panel->addElement(deathColourLabel);

    m_panel->addNewHalfLine();
    m_deathRed = std::make_shared<UISlider>("R", COLOUR_RANGE, THREE_VAL_SIZE);
    m_deathRed->setDescription("Set the amount of red that a particle's colour will end with.");
    m_panel->addElement(m_deathRed);

    m_deathGreen = std::make_shared<UISlider>("G", COLOUR_RANGE, THREE_VAL_SIZE);
    m_deathGreen->setDescription("Set the amount of green that a particle's colour will end with.");
    m_panel->addElement(m_deathGreen);

    m_deathBlue = std::make_shared<UISlider>("B", COLOUR_RANGE, THREE_VAL_SIZE);
    m_deathBlue->setDescription("Set the amount of blue that a particle's colour will end with.");
    m_panel->addElement(m_deathBlue);

    m_panel->addNewHalfLine();
    m_deathOpacity = std::make_shared<UISlider>("Opacity", PERCENT_RANGE, ONE_VAL_SIZE);
    m_deathOpacity->setDescription("Set the level of transparency that a particle will end with.");
    m_panel->addElement(m_deathOpacity);

    m_panel->addNewLine();
    auto textureLabel{ std::make_shared<UIText>("Particle Texture", LABEL_SIZE) };
    m_panel->addElement(textureLabel);

    m_panel->addNewHalfLine();
    m_textureName = std::make_shared<UIField>("", ONE_VAL_SIZE);
    m_textureName->setDescription("Specify the name of the texture image file that this emitter's particles will use. This file should be located in 'assets/texture/'.");
    m_panel->addElement(m_textureName);

    m_panel->addNewHalfLine();
    auto textureButton{ std::make_shared<UIButton>("Load",
        BUTTON_SIZE, false, [this]()
        {
            prepareToLoadTexture();
        }
    ) };
    m_panel->addElement(textureButton);

    m_panel->addToRenderer(uRenderer, tRenderer);
    m_panel->setEnabled(false);
}

void VisualsPanel::update(float deltaTime, Emitter &emitter)
{
    // Set the blend mode.
    emitter.setBlendMode(m_linearBlend->isToggled() ? 
        Emitter::BlendMode::Linear : Emitter::BlendMode::Additive);

    // Set colour.
    float red{ m_red->getValue() / COLOUR_RANGE.y };
    float green{ m_green->getValue() / COLOUR_RANGE.y };
    float blue{ m_blue->getValue() / COLOUR_RANGE.y };
    float alpha{ m_opacity->getValue() / PERCENT_RANGE.y };
    emitter.setColour(glm::vec4(red, green, blue, alpha));

    // Set birth colour.
    float birthRed{ m_birthRed->getValue() / COLOUR_RANGE.y };
    float birthGreen{ m_birthGreen->getValue() / COLOUR_RANGE.y };
    float birthBlue{ m_birthBlue->getValue() / COLOUR_RANGE.y };
    float birthAlpha{ m_birthOpacity->getValue() / PERCENT_RANGE.y };
    emitter.setBirthColour(glm::vec4(birthRed, birthGreen, birthBlue, birthAlpha));

    // Set death colour.
    float deathRed{ m_deathRed->getValue() / COLOUR_RANGE.y };
    float deathGreen{ m_deathGreen->getValue() / COLOUR_RANGE.y };
    float deathBlue{ m_deathBlue->getValue() / COLOUR_RANGE.y };
    float deathAlpha{ m_deathOpacity->getValue() / PERCENT_RANGE.y };
    emitter.setDeathColour(glm::vec4(deathRed, deathGreen, deathBlue, deathAlpha));

    // Set emitter texture.
    if (m_hasNewTexture)
    {
        m_hasNewTexture = false;

        std::string textureName;
        m_textureName->getValue(textureName);
        emitter.setTexture(m_assetLoader, textureName);
    }
}

void VisualsPanel::updateUIFromEmitter(const Emitter &emitter)
{
    bool m_isLinearBlend{ emitter.getBlendMode() == Emitter::BlendMode::Linear };
    m_linearBlend->setToggled(m_isLinearBlend);
    m_additiveBlend->setToggled(!m_isLinearBlend);

    glm::vec4 colour{ emitter.getColour() };
    m_red->setValue(static_cast<int>(colour.r * COLOUR_RANGE.y));
    m_green->setValue(static_cast<int>(colour.g * COLOUR_RANGE.y));
    m_blue->setValue(static_cast<int>(colour.b * COLOUR_RANGE.y));
    m_opacity->setValue(static_cast<int>(colour.a * PERCENT_RANGE.y));

    glm::vec4 birthColour{ emitter.getBirthColour() };
    m_birthRed->setValue(static_cast<int>(birthColour.r * COLOUR_RANGE.y));
    m_birthGreen->setValue(static_cast<int>(birthColour.g * COLOUR_RANGE.y));
    m_birthBlue->setValue(static_cast<int>(birthColour.b * COLOUR_RANGE.y));
    m_birthOpacity->setValue(static_cast<int>(birthColour.a * PERCENT_RANGE.y));

    glm::vec4 deathColour{ emitter.getDeathColour() };
    m_deathRed->setValue(static_cast<int>(deathColour.r * COLOUR_RANGE.y));
    m_deathGreen->setValue(static_cast<int>(deathColour.g * COLOUR_RANGE.y));
    m_deathBlue->setValue(static_cast<int>(deathColour.b * COLOUR_RANGE.y));
    m_deathOpacity->setValue(static_cast<int>(deathColour.a * PERCENT_RANGE.y));

    const std::string &textureName{ emitter.getTextureName() };
    m_textureName->setValue(textureName);
}

void VisualsPanel::prepareToLoadTexture()
{
    m_hasNewTexture = true;
}
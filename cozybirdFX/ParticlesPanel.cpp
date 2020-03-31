#include "ParticlesPanel.h"
#include "EditorState.h"
#include "Emitter.h"
#include "UIButton.h"
#include "UISlider.h"
#include "UIText.h"
#include "UIIntField.h"
#include "UIFloatField.h"

#include <iostream>

ParticlesPanel::ParticlesPanel(const EditorState &state, 
    SpriteRenderer &sRenderer, TextRenderer &tRenderer,  
    UIRenderer &uRenderer) :
    m_state(state)
{
    m_description = "Right-click to move the current emitter.";

    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    m_delay = std::make_shared<UIFloatField>("Delay Before Start", 
        UIField::MID_MAX_CHARS, ONE_VAL_SIZE);
    m_delay->setDescription("The duration of time in seconds before emitting particles, from the start of each animation loop.");
    m_panel->addElement(m_delay);

    m_panel->addNewLine();
    m_duration = std::make_shared<UIFloatField>("Emitter Duration", 
        UIField::MID_MAX_CHARS, ONE_VAL_SIZE);
    m_duration->setDescription("The duration of time in seconds before this emitter stops emitting particles, from the start of each animation loop.");
    m_panel->addElement(m_duration);

    m_panel->addNewLine();
    auto posLabel{ std::make_shared<UIText>("Position", LABEL_SIZE) };
    m_panel->addElement(posLabel);

    m_panel->addNewHalfLine();
    m_xPosition = std::make_shared<UIFloatField>("x", UIField::MID_MAX_CHARS,
        TWO_VAL_SIZE);
    m_xPosition->setDescription("The x-coordinate of the emitter's origin position.");
    m_panel->addElement(m_xPosition);

    m_yPosition = std::make_shared<UIFloatField>("y", UIField::MID_MAX_CHARS,
        TWO_VAL_SIZE);
    m_yPosition->setDescription("The y-coordinate of the emitter's origin position.");
    m_panel->addElement(m_yPosition);

    m_panel->addNewLine();
    auto distLabel{ std::make_shared<UIText>("Distribution", LABEL_SIZE) };
    m_panel->addElement(distLabel);

    m_panel->addNewHalfLine();
    m_distWidth = std::make_shared<UIFloatField>("Width", UIField::MID_MAX_CHARS, 
        TWO_VAL_SIZE);
    m_distWidth->setDescription("The width of the box to distrbute the emitted particles in.");
    m_panel->addElement(m_distWidth);

    m_distHeight = std::make_shared<UIFloatField>("Height", UIField::MID_MAX_CHARS, 
        TWO_VAL_SIZE);
    m_distHeight->setDescription("The height of the box to distrbute the emitted particles in.");
    m_panel->addElement(m_distHeight);

    m_panel->addNewLine();
    m_numToGenerate = std::make_shared<UIIntField>("Particle Amount", UIField::MID_MAX_CHARS,
        ONE_VAL_SIZE);
    m_numToGenerate->setDescription("The number of particles to generate per emission.");
    m_panel->addElement(m_numToGenerate);

    m_panel->addNewLine();
    m_spawnTime = std::make_shared<UIFloatField>("Spawn Interval", UIField::MID_MAX_CHARS,
        ONE_VAL_SIZE);
    m_spawnTime->setDescription("The delay in seconds between each emission.");
    m_panel->addElement(m_spawnTime);

    m_panel->addNewLine();
    auto lifeLabel{ std::make_shared<UIText>("Particle Life", LABEL_SIZE) };
    m_panel->addElement(lifeLabel);

    m_panel->addNewHalfLine();
    m_lifeMin = std::make_shared<UIFloatField>("Min", UIField::MID_MAX_CHARS, 
        TWO_VAL_SIZE);
    m_lifeMin->setDescription("The minimum duration in seconds that a particle can live for.");
    m_panel->addElement(m_lifeMin);

    m_lifeMax = std::make_shared<UIFloatField>("Max", UIField::MID_MAX_CHARS, 
        TWO_VAL_SIZE);
    m_lifeMax->setDescription("The maximum duration in seconds that a particle can live for.");
    m_panel->addElement(m_lifeMax);

    m_panel->addNewLine();
    auto sizeLabel{ std::make_shared<UIText>("Particle Size", LABEL_SIZE) };
    m_panel->addElement(sizeLabel);

    m_panel->addNewHalfLine();
    m_sizeMin = std::make_shared<UIFloatField>("Min", UIField::SMALL_MAX_CHARS, 
        THREE_VAL_SIZE);
    m_sizeMin->setDescription("The minimum size that a particle can be created with.");
    m_panel->addElement(m_sizeMin);

    m_sizeMax = std::make_shared<UIFloatField>("Max", UIField::SMALL_MAX_CHARS, 
        THREE_VAL_SIZE);
    m_sizeMax->setDescription("The maximum size that a particle can be created with.");
    m_panel->addElement(m_sizeMax);

    m_sizeGrowth = std::make_shared<UIFloatField>("Growth", UIField::SMALL_MAX_CHARS, 
        THREE_VAL_SIZE);
    m_sizeGrowth->setDescription("The rate at which a particle's size will grow by.");
    m_panel->addElement(m_sizeGrowth);

    m_panel->addNewLine();
    auto rotLabel{ std::make_shared<UIText>("Rotation", LABEL_SIZE) };
    m_panel->addElement(rotLabel);

    m_panel->addNewHalfLine();
    m_facingDir = std::make_shared<UIButton>("Face Movement Direction",
        ONE_BUTTON_SIZE, true, []() {});
    m_facingDir->setUndoAction([]() {});
    m_facingDir->setDescription("Specify whether the particle's rotation should face its direction of movement. Enabling this will disable rotation settings.");
    m_panel->addElement(m_facingDir);

    m_panel->addNewHalfLine();
    m_rotationMin = std::make_shared<UISlider>("Min", ANGLE_RANGE, THREE_VAL_SIZE);
    m_rotationMin->setDescription("The minimum rotation angle that a particle can be created with.");
    m_panel->addElement(m_rotationMin);

    m_rotationMax = std::make_shared<UISlider>("Max", ANGLE_RANGE, THREE_VAL_SIZE);
    m_rotationMax->setDescription("The maximum rotation angle that a particle can be created with.");
    m_panel->addElement(m_rotationMax);

    m_rotationGrowth = std::make_shared<UISlider>("Growth", ANGLE_RANGE, THREE_VAL_SIZE);
    m_rotationGrowth->setDescription("The rate at which a particle's rotation angle will grow by.");
    m_panel->addElement(m_rotationGrowth);

    m_panel->addToRenderer(sRenderer, uRenderer, tRenderer);
}

void ParticlesPanel::update(float deltaTime, Emitter &emitter)
{
    // Move emitter by right click.
    if (m_panel->isEnabled() && IEditorPanel::hasClicked())
    {
        glm::vec2 clickedPos{ m_state.screenToClip(m_clickedPos) };
        m_xPosition->setValue(clickedPos.x);
        m_yPosition->setValue(clickedPos.y);
    }

    // Show/hide rotation options depending on facing button toggle state.
    bool isRotation{ m_panel->isEnabled() && !m_facingDir->isToggled() };
    m_rotationMin->setEnabled(isRotation);
    m_rotationMax->setEnabled(isRotation);
    m_rotationGrowth->setEnabled(isRotation);

    // Set delay time.
    float delay;
    if (m_delay->getValue(delay))
    {
        emitter.setDelayBeforeStart(delay);
    }

    // Set emitter duration.
    float duration;
    if (m_duration->getValue(duration))
    {
        emitter.setEmitterDuration(duration);
    }

    // Set position.
    glm::vec2 pos;
    bool isPosX{ m_xPosition->getValue(pos.x) };
    bool isPosY{ m_yPosition->getValue(pos.y) };
    if (isPosX || isPosY)
    {
        emitter.setPosition(pos);
    }

    // Set distribution.
    glm::vec2 distribution;
    bool isDistWidth{ m_distWidth->getValue(distribution.x) };
    bool isDistHeight{ m_distHeight->getValue(distribution.y) };
    if (isDistWidth || isDistHeight)
    {
        emitter.setDistribution(distribution);
    }

    // Set number of particles to generate.
    int numToGen;
    if (m_numToGenerate->getValue(numToGen))
    {
        emitter.setNumToGenerate(numToGen);
    }

    // Set spawn time.
    float spawnTime;
    if (m_spawnTime->getValue(spawnTime))
    {
        emitter.setTimeToSpawn(spawnTime);
    }
    
    // Set if facing direction of movement.
    emitter.setIsFacingDirection(m_facingDir->isToggled());

    // Set minimum rotation.
    int rotationMin{ m_rotationMin->getValue() };
    emitter.setRotationMin(rotationMin);

    // Set maximum rotation.
    int rotationMax{ m_rotationMax->getValue() };
    emitter.setRotationMax(rotationMax);

    // Set rotation growth rate.
    int rotationGrowth{ m_rotationGrowth->getValue() };
    emitter.setRotationGrowth(rotationGrowth);

    // Set minimum particle size.
    float sizeMin;
    if (m_sizeMin->getValue(sizeMin))
    {
        emitter.setSizeMin(sizeMin);
    }

    // Set maximum particle size.
    float sizeMax;
    if (m_sizeMax->getValue(sizeMax))
    {
        emitter.setSizeMax(sizeMax);
    }

    // Set particle size growth rate.
    float sizeGrowth;
    if (m_sizeGrowth->getValue(sizeGrowth))
    {
        emitter.setSizeGrowth(sizeGrowth);
    }

    // Set duration.
    float lifeMin;
    if (m_lifeMin->getValue(lifeMin))
    {
        emitter.setLifeMin(lifeMin);
    }

    // Convert max to offset.
    float lifeMax;
    if (m_lifeMax->getValue(lifeMax))
    {
        emitter.setLifeMax(lifeMax);
    }
}

void ParticlesPanel::updateUIFromEmitter(const Emitter &emitter)
{
    m_delay->setValue(emitter.getDelayBeforeStart());
    m_duration->setValue(emitter.getEmitterDuration());

    glm::vec2 pos{ emitter.getPosition() };
    m_xPosition->setValue(pos.x);
    m_yPosition->setValue(pos.y);

    glm::vec2 distribution{ emitter.getDistribution() };
    m_distWidth->setValue(distribution.x);
    m_distHeight->setValue(distribution.y);

    m_numToGenerate->setValue(emitter.getNumToGenerate());
    m_spawnTime->setValue(emitter.getTimeToSpawn());

    m_facingDir->setToggled(emitter.isFacingDirection());

    glm::ivec3 rotation{ emitter.getRotation() };
    m_rotationMin->setValue(rotation.x);
    m_rotationMax->setValue(rotation.y);
    m_rotationGrowth->setValue(rotation.z);

    glm::vec3 size{ emitter.getSize() };
    m_sizeMin->setValue(size.x);
    m_sizeMax->setValue(size.y);
    m_sizeGrowth->setValue(size.z);

    glm::vec2 life{ emitter.getLife() };
    m_lifeMin->setValue(life.x);
    m_lifeMax->setValue(life.y);
}

#include "ParticlesPanel.h"
#include "Emitter.h"
#include "UIText.h"
#include "UITextField.h"

ParticlesPanel::ParticlesPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    m_delay = std::make_shared<UITextField>("Delay Before Start", ONE_VAL_SIZE);
    m_panel->addElement(m_delay);

    m_panel->addNewLine();
    m_duration = std::make_shared<UITextField>("Emitter Duration", ONE_VAL_SIZE);
    m_panel->addElement(m_duration);

    m_panel->addNewLine();
    auto posLabel{ std::make_shared<UIText>("Position", LABEL_SIZE) };
    m_panel->addElement(posLabel);

    m_panel->addNewHalfLine();
    m_xPosition = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_panel->addElement(m_xPosition);

    m_yPosition = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_panel->addElement(m_yPosition);

    m_panel->addNewLine();
    m_numToGenerate = std::make_shared<UITextField>("Particle Density", ONE_VAL_SIZE);
    m_panel->addElement(m_numToGenerate);

    m_panel->addNewLine();
    m_spawnTime = std::make_shared<UITextField>("Time to Spawn", ONE_VAL_SIZE);
    m_panel->addElement(m_spawnTime);

    m_panel->addNewLine();
    m_size = std::make_shared<UITextField>("Particle Size", ONE_VAL_SIZE);
    m_panel->addElement(m_size);

    m_panel->addNewLine();
    auto lifeLabel{ std::make_shared<UIText>("Particle Life", LABEL_SIZE) };
    m_panel->addElement(lifeLabel);

    m_panel->addNewHalfLine();
    m_lifeMin = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_panel->addElement(m_lifeMin);

    m_lifeMax = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_panel->addElement(m_lifeMax);

    m_panel->addNewLine();
    auto velXLabel{ std::make_shared<UIText>("Velocity.x", LABEL_SIZE) };
    m_panel->addElement(velXLabel);

    m_panel->addNewHalfLine();
    m_velXMin = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_panel->addElement(m_velXMin);

    m_velXMax = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_panel->addElement(m_velXMax);

    m_panel->addNewLine();
    auto velYLabel{ std::make_shared<UIText>("Velocity.y", LABEL_SIZE) };
    m_panel->addElement(velYLabel);

    m_panel->addNewHalfLine();
    m_velYMin = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_panel->addElement(m_velYMin);

    m_velYMax = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_panel->addElement(m_velYMax);

    m_panel->addNewLine();
    auto accelLabel{ std::make_shared<UIText>("Acceleration", LABEL_SIZE) };
    m_panel->addElement(accelLabel);

    m_panel->addNewHalfLine();
    m_accelX = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_panel->addElement(m_accelX);

    m_accelY = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_panel->addElement(m_accelY);

    m_panel->addToRenderer(uRenderer, tRenderer);
}

void ParticlesPanel::update(Emitter *emitter, float deltaTime)
{
    // Set delay time.
    float delay;
    if (m_delay->getValue(delay))
    {
        emitter->setDelayBeforeStart(delay);
    }

    // Set emitter duration.
    float duration;
    if (m_duration->getValue(duration))
    {
        emitter->setEmitterDuration(duration);
    }

    // Set position.
    glm::vec2 pos;
    bool isPosX{ m_xPosition->getValue(pos.x) };
    bool isPosY{ m_yPosition->getValue(pos.y) };
    if (isPosX || isPosY)
    {
        emitter->setPosition(pos);
    }

    // Set number of particles to generate.
    int numToGen;
    if (m_numToGenerate->getValue(numToGen))
    {
        emitter->setNumToGenerate(numToGen);
    }

    // Set spawn time.
    float spawnTime;
    if (m_spawnTime->getValue(spawnTime))
    {
        emitter->setTimeToSpawn(spawnTime);
    }

    // Set velocity.
    glm::vec2 velMin;
    bool isVelXMin{ m_velXMin->getValue(velMin.x) };
    bool isVelYMin{ m_velYMin->getValue(velMin.y) };
    if (isVelXMin || isVelYMin)
    {
        emitter->setVelocityMin(velMin);
    }

    // Convert max to offset.
    glm::vec2 velMax;
    bool isVelXMax{ m_velXMax->getValue(velMax.x) };
    bool isVelYMax{ m_velYMax->getValue(velMax.y) };
    if (isVelXMin || isVelYMin || isVelXMax || isVelYMax)
    {
        glm::vec2 velOffset{ velMax.x - velMin.x, velMax.y - velMin.y };
        velOffset = glm::clamp(velOffset, { 0.f, 0.f }, velOffset);
        emitter->setVelocityOffset(velOffset);
    }

    // Set acceleration.
    glm::vec2 accel;
    bool isAccelX{ m_accelX->getValue(accel.x) };
    bool isAccelY{ m_accelY->getValue(accel.y) };
    if (isAccelX || isAccelY)
    {
        emitter->setAcceleration(accel);
    }

    // Set particle size.
    float size;
    if (m_size->getValue(size))
    {
        emitter->setSize(size);
    }

    // Set duration.
    float lifeMin;
    bool isLifeMin{ m_lifeMin->getValue(lifeMin) };
    if (isLifeMin)
    {
        emitter->setLifeMin(lifeMin);
    }

    // Convert max to offset.
    float lifeMax;
    bool isLifeMax{ m_lifeMax->getValue(lifeMax) };
    if (isLifeMin || isLifeMax)
    {
        float lifeOffset{ lifeMax - lifeMin };
        lifeOffset = glm::clamp(lifeOffset, 0.f, lifeOffset);
        emitter->setLifeOffset(lifeOffset);
    }
}

void ParticlesPanel::updateUIFromEmitter(Emitter *emitter)
{
    m_delay->setValue(emitter->getDelayBeforeStart());
    m_duration->setValue(emitter->getEmitterDuration());

    glm::vec2 pos{ emitter->getPosition() };
    m_xPosition->setValue(pos.x);
    m_yPosition->setValue(pos.y);

    m_numToGenerate->setValue(emitter->getNumToGenerate());
    m_spawnTime->setValue(emitter->getTimeToSpawn());

    glm::vec2 velMin{ emitter->getVelocityMin() };
    glm::vec2 velOffset{ emitter->getVelocityOffset() };
    m_velXMin->setValue(velMin.x);
    m_velXMax->setValue(velMin.x + velOffset.x);
    m_velYMin->setValue(velMin.y);
    m_velYMax->setValue(velMin.y + velOffset.y);

    glm::vec2 accel{ emitter->getAcceleration() };
    m_accelX->setValue(accel.x);
    m_accelY->setValue(accel.y);

    m_size->setValue(emitter->getSize());

    float durationMin{ emitter->getLifeMin() };
    m_lifeMin->setValue(durationMin);
    m_lifeMax->setValue(durationMin + emitter->getLifeOffset());
}

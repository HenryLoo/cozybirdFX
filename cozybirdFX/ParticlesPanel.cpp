#include "ParticlesPanel.h"
#include "Emitter.h"
#include "UIText.h"
#include "UITextField.h"

ParticlesPanel::ParticlesPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    m_delayField = std::make_shared<UITextField>("Delay Before Start", ONE_VAL_SIZE);
    m_panel->addElement(m_delayField);

    m_panel->addNewLine();
    m_emDurationField = std::make_shared<UITextField>("Emitter Duration", ONE_VAL_SIZE);
    m_panel->addElement(m_emDurationField);

    m_panel->addNewLine();
    auto posLabel{ std::make_shared<UIText>("Position", LABEL_SIZE) };
    m_panel->addElement(posLabel);

    m_panel->addNewHalfLine();
    m_xField = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_panel->addElement(m_xField);

    m_yField = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_panel->addElement(m_yField);

    m_panel->addNewLine();
    m_numGenField = std::make_shared<UITextField>("Particle Density", ONE_VAL_SIZE);
    m_panel->addElement(m_numGenField);

    m_panel->addNewLine();
    m_spawnTimeField = std::make_shared<UITextField>("Time to Spawn", ONE_VAL_SIZE);
    m_panel->addElement(m_spawnTimeField);

    m_panel->addNewLine();
    m_sizeField = std::make_shared<UITextField>("Particle Size", ONE_VAL_SIZE);
    m_panel->addElement(m_sizeField);

    m_panel->addNewLine();
    auto lifeLabel{ std::make_shared<UIText>("Particle Life", LABEL_SIZE) };
    m_panel->addElement(lifeLabel);

    m_panel->addNewHalfLine();
    m_lifeMinField = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_panel->addElement(m_lifeMinField);

    m_lifeMaxField = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_panel->addElement(m_lifeMaxField);

    m_panel->addNewLine();
    auto velXLabel{ std::make_shared<UIText>("Velocity.x", LABEL_SIZE) };
    m_panel->addElement(velXLabel);

    m_panel->addNewHalfLine();
    m_velXMinField = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_panel->addElement(m_velXMinField);

    m_velXMaxField = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_panel->addElement(m_velXMaxField);

    m_panel->addNewLine();
    auto velYLabel{ std::make_shared<UIText>("Velocity.y", LABEL_SIZE) };
    m_panel->addElement(velYLabel);

    m_panel->addNewHalfLine();
    m_velYMinField = std::make_shared<UITextField>("Min", TWO_VAL_SIZE);
    m_panel->addElement(m_velYMinField);

    m_velYMaxField = std::make_shared<UITextField>("Max", TWO_VAL_SIZE);
    m_panel->addElement(m_velYMaxField);

    m_panel->addNewLine();
    auto accelLabel{ std::make_shared<UIText>("Acceleration", LABEL_SIZE) };
    m_panel->addElement(accelLabel);

    m_panel->addNewHalfLine();
    m_accelXField = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_panel->addElement(m_accelXField);

    m_accelYField = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_panel->addElement(m_accelYField);

    m_panel->addToRenderer(uRenderer, tRenderer);
}

void ParticlesPanel::update(Emitter *emitter, float deltaTime)
{
    // Set delay time.
    float delay;
    if (m_delayField->getValue(delay))
    {
        emitter->setDelayBeforeStart(delay);
    }

    // Set emitter duration.
    float duration;
    if (m_emDurationField->getValue(duration))
    {
        emitter->setEmitterDuration(duration);
    }

    // Set position.
    glm::vec2 pos;
    bool isPosX{ m_xField->getValue(pos.x) };
    bool isPosY{ m_yField->getValue(pos.y) };
    if (isPosX || isPosY)
    {
        emitter->setPosition(pos);
    }

    // Set number of particles to generate.
    int numToGen;
    if (m_numGenField->getValue(numToGen))
    {
        emitter->setNumToGenerate(numToGen);
    }

    // Set spawn time.
    float spawnTime;
    if (m_spawnTimeField->getValue(spawnTime))
    {
        emitter->setTimeToSpawn(spawnTime);
    }

    // Set velocity.
    glm::vec2 velMin;
    bool isVelXMin{ m_velXMinField->getValue(velMin.x) };
    bool isVelYMin{ m_velYMinField->getValue(velMin.y) };
    if (isVelXMin || isVelYMin)
    {
        emitter->setVelocityMin(velMin);
    }

    // Convert max to offset.
    glm::vec2 velMax;
    bool isVelXMax{ m_velXMaxField->getValue(velMax.x) };
    bool isVelYMax{ m_velYMaxField->getValue(velMax.y) };
    if (isVelXMin || isVelYMin || isVelXMax || isVelYMax)
    {
        glm::vec2 velOffset{ velMax.x - velMin.x, velMax.y - velMin.y };
        velOffset = glm::clamp(velOffset, { 0.f, 0.f }, velOffset);
        emitter->setVelocityOffset(velOffset);
    }

    // Set acceleration.
    glm::vec2 accel;
    bool isAccelX{ m_accelXField->getValue(accel.x) };
    bool isAccelY{ m_accelYField->getValue(accel.y) };
    if (isAccelX || isAccelY)
    {
        emitter->setAcceleration(accel);
    }

    // Set particle size.
    float size;
    if (m_sizeField->getValue(size))
    {
        emitter->setSize(size);
    }

    // Set duration.
    float lifeMin;
    bool isLifeMin{ m_lifeMinField->getValue(lifeMin) };
    if (isLifeMin)
    {
        emitter->setLifeMin(lifeMin);
    }

    // Convert max to offset.
    float lifeMax;
    bool isLifeMax{ m_lifeMaxField->getValue(lifeMax) };
    if (isLifeMin || isLifeMax)
    {
        float lifeOffset{ lifeMax - lifeMin };
        lifeOffset = glm::clamp(lifeOffset, 0.f, lifeOffset);
        emitter->setLifeOffset(lifeOffset);
    }
}

void ParticlesPanel::updateUIFromEmitter(Emitter *emitter)
{
    m_delayField->setValue(emitter->getDelayBeforeStart());
    m_emDurationField->setValue(emitter->getEmitterDuration());

    glm::vec2 pos{ emitter->getPosition() };
    m_xField->setValue(pos.x);
    m_yField->setValue(pos.y);

    m_numGenField->setValue(emitter->getNumToGenerate());
    m_spawnTimeField->setValue(emitter->getTimeToSpawn());

    glm::vec2 velMin{ emitter->getVelocityMin() };
    glm::vec2 velOffset{ emitter->getVelocityOffset() };
    m_velXMinField->setValue(velMin.x);
    m_velXMaxField->setValue(velMin.x + velOffset.x);
    m_velYMinField->setValue(velMin.y);
    m_velYMaxField->setValue(velMin.y + velOffset.y);

    glm::vec2 accel{ emitter->getAcceleration() };
    m_accelXField->setValue(accel.x);
    m_accelYField->setValue(accel.y);

    m_sizeField->setValue(emitter->getSize());

    float durationMin{ emitter->getLifeMin() };
    m_lifeMinField->setValue(durationMin);
    m_lifeMaxField->setValue(durationMin + emitter->getLifeOffset());
}

#include "MovementPanel.h"
#include "Emitter.h"
#include "UIButton.h"
#include "UISlider.h"
#include "UIText.h"
#include "UIFloatField.h"

MovementPanel::MovementPanel(SpriteRenderer &sRenderer, 
    TextRenderer &tRenderer, UIRenderer &uRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    m_panel->addNewLine();
    auto speedLabel{ std::make_shared<UIText>("Particle Speed", LABEL_SIZE) };
    m_panel->addElement(speedLabel);

    m_panel->addNewHalfLine();
    m_speedMin = std::make_shared<UIFloatField>("Min", THREE_VAL_SIZE);
    m_speedMin->setDescription("The minimum speed that a particle can move at.");
    m_panel->addElement(m_speedMin);

    m_speedMax = std::make_shared<UIFloatField>("Max", THREE_VAL_SIZE);
    m_speedMax->setDescription("The maximum speed that a particle can move at.");
    m_panel->addElement(m_speedMax);

    m_speedGrowth = std::make_shared<UIFloatField>("Growth", THREE_VAL_SIZE);
    m_speedGrowth->setDescription("The rate at which a particle's speed will grow by.");
    m_panel->addElement(m_speedGrowth);

    m_panel->addNewLine();
    auto dirLabel{ std::make_shared<UIText>("Particle Direction", LABEL_SIZE) };
    m_panel->addElement(dirLabel);

    m_panel->addNewHalfLine();
    m_directionMin = std::make_shared<UISlider>("Min", ANGLE_RANGE, THREE_VAL_SIZE);
    m_directionMin->setDescription("The minimum angle that a particle can move toward.");
    m_panel->addElement(m_directionMin);

    m_directionMax = std::make_shared<UISlider>("Max", ANGLE_RANGE, THREE_VAL_SIZE);
    m_directionMax->setDescription("The maximum angle that a particle can move toward.");
    m_panel->addElement(m_directionMax);

    m_directionGrowth = std::make_shared<UISlider>("Growth", ANGLE_RANGE, THREE_VAL_SIZE);
    m_directionGrowth->setDescription("The rate at which a particle's direction angle will grow by.");
    m_panel->addElement(m_directionGrowth);

    //m_panel->addNewLine();
    //auto pathLabel{ std::make_shared<UIText>("Particle Path", LABEL_SIZE) };
    //m_panel->addElement(pathLabel);

    //m_panel->addNewHalfLine();
    //auto leftPtLabel{ std::make_shared<UIText>("> Left click to move spline point.", LABEL_SIZE) };
    //m_panel->addElement(leftPtLabel);

    //m_panel->addNewHalfLine();
    //auto rightPtLabel{ std::make_shared<UIText>("> Right click to add spline point.", LABEL_SIZE) };
    //m_panel->addElement(rightPtLabel);

    //m_panel->addNewHalfLine();
    //auto deletePathButton{ std::make_shared<UIButton>("Delete Path", ONE_BUTTON_SIZE) };
    //m_panel->addElement(deletePathButton);

    m_panel->addNewLine();
    auto oscillateLabel{ std::make_shared<UIText>("Emitter Movement", LABEL_SIZE) };
    m_panel->addElement(oscillateLabel);

    m_panel->addNewLine();
    auto hSinLabel{ std::make_shared<UIText>("Horizontal Sine", LABEL_SIZE) };
    m_panel->addElement(hSinLabel);

    m_panel->addNewHalfLine();
    m_hSineAmplitude = std::make_shared<UIFloatField>("Amplitude", TWO_VAL_SIZE);
    m_hSineAmplitude->setDescription("The maximum horizontal distance that the emitter will oscillate to.");
    m_panel->addElement(m_hSineAmplitude);

    m_hSinePeriod = std::make_shared<UIFloatField>("Period", TWO_VAL_SIZE);
    m_hSinePeriod->setDescription("The duration of time in seconds that is required to oscillate the emitter to the horizontal amplitude.");
    m_panel->addElement(m_hSinePeriod);

    m_panel->addNewLine();
    auto vSinLabel{ std::make_shared<UIText>("Vertical Sine", LABEL_SIZE) };
    m_panel->addElement(vSinLabel);

    m_panel->addNewHalfLine();
    m_vSineAmplitude = std::make_shared<UIFloatField>("Amplitude", TWO_VAL_SIZE);
    m_vSineAmplitude->setDescription("The maximum vertical distance that the emitter will oscillate to.");
    m_panel->addElement(m_vSineAmplitude);

    m_vSinePeriod = std::make_shared<UIFloatField>("Period", TWO_VAL_SIZE);
    m_vSinePeriod->setDescription("The duration of time in seconds that is required to oscillate the emitter to the vertical amplitude.");
    m_panel->addElement(m_vSinePeriod);

    m_panel->addNewLine();
    auto circleLabel{ std::make_shared<UIText>("Circle", LABEL_SIZE) };
    m_panel->addElement(circleLabel);

    m_panel->addNewHalfLine();
    m_circleRadius = std::make_shared<UIFloatField>("Radius", TWO_VAL_SIZE);
    m_circleRadius->setDescription("The radius of the circle that the emitter will rotate around.");
    m_panel->addElement(m_circleRadius);

    m_circlePeriod = std::make_shared<UIFloatField>("Period", TWO_VAL_SIZE);
    m_circlePeriod->setDescription("The duration of time in seconds that is required to perform one rotation around the circle.");
    m_panel->addElement(m_circlePeriod);

    m_panel->addToRenderer(sRenderer, uRenderer, tRenderer);
    m_panel->setEnabled(false);
}

void MovementPanel::update(float deltaTime, Emitter &emitter)
{
    // Set minimum speed.
    float speedMin;
    if (m_speedMin->getValue(speedMin))
    {
        emitter.setSpeedMin(speedMin);
    }

    // Set maximum speed.
    float speedMax;
    if (m_speedMax->getValue(speedMax))
    {
        emitter.setSpeedMax(speedMax);
    }

    // Set speed growth rate.
    float speedGrowth;
    if (m_speedGrowth->getValue(speedGrowth))
    {
        emitter.setSpeedGrowth(speedGrowth);
    }

    // Set minimum direction.
    int directionMin{ m_directionMin->getValue() };
    emitter.setDirectionMin(directionMin);

    // Set maximum direction.
    int directionMax{ m_directionMax->getValue() };
    emitter.setDirectionMax(directionMax);

    // Set direction growth rate.
    int directionGrowth{ m_directionGrowth->getValue() };
    emitter.setDirectionGrowth(directionGrowth);

    // Set horizontal sine movement amplitude.
    float hSinAmp;
    bool isHSinAmp{ m_hSineAmplitude->getValue(hSinAmp) };
    if (isHSinAmp)
    {
        emitter.setHSineAmplitude(hSinAmp);
    }

    // Set horizontal sine movement period.
    float hSinPeriod;
    bool isHSinPeriod{ m_hSinePeriod->getValue(hSinPeriod) };
    if (isHSinPeriod)
    {
        emitter.setHSinePeriod(hSinPeriod);
    }

    // Set vertical sine movement amplitude.
    float vSinAmp;
    bool isVSinAmp{ m_vSineAmplitude->getValue(vSinAmp) };
    if (isVSinAmp)
    {
        emitter.setVSineAmplitude(vSinAmp);
    }

    // Set vertical sine movement period.
    float vSinPeriod;
    bool isVSinPeriod{ m_vSinePeriod->getValue(vSinPeriod) };
    if (isVSinPeriod)
    {
        emitter.setVSinePeriod(vSinPeriod);
    }

    // Set circle movement radius.
    float circleRadius;
    bool isCircleRadius{ m_circleRadius->getValue(circleRadius) };
    if (isCircleRadius)
    {
        emitter.setCircleRadius(circleRadius);
    }

    // Set circle movement period.
    float circlePeriod;
    bool isCirclePeriod{ m_circlePeriod->getValue(circlePeriod) };
    if (isCirclePeriod)
    {
        emitter.setCirclePeriod(circlePeriod);
    }
}

void MovementPanel::updateUIFromEmitter(const Emitter &emitter)
{
    glm::vec3 speed{ emitter.getSpeed() };
    m_speedMin->setValue(speed.x);
    m_speedMax->setValue(speed.y);
    m_speedGrowth->setValue(speed.z);

    glm::ivec3 direction{ emitter.getDirection() };
    m_directionMin->setValue(direction.x);
    m_directionMax->setValue(direction.y);
    m_directionGrowth->setValue(direction.z);

    m_hSineAmplitude->setValue(emitter.getHSineAmplitude());
    m_hSinePeriod->setValue(emitter.getHSinePeriod());

    m_vSineAmplitude->setValue(emitter.getVSineAmplitude());
    m_vSinePeriod->setValue(emitter.getVSinePeriod());

    m_circleRadius->setValue(emitter.getCircleRadius());
    m_circlePeriod->setValue(emitter.getCirclePeriod());
}

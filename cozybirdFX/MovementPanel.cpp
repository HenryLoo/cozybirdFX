#include "MovementPanel.h"
#include "Emitter.h"
#include "UIButton.h"
#include "UIText.h"
#include "UITextField.h"

MovementPanel::MovementPanel(TextRenderer &tRenderer, UIRenderer &uRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    auto pathLabel{ std::make_shared<UIText>("Particle Path", LABEL_SIZE) };
    m_panel->addElement(pathLabel);

    m_panel->addNewHalfLine();
    auto leftPtLabel{ std::make_shared<UIText>("> Left click to move spline point.", LABEL_SIZE) };
    m_panel->addElement(leftPtLabel);

    m_panel->addNewHalfLine();
    auto rightPtLabel{ std::make_shared<UIText>("> Right click to add spline point.", LABEL_SIZE) };
    m_panel->addElement(rightPtLabel);

    m_panel->addNewHalfLine();
    m_particleSpeed = std::make_shared<UITextField>("Particle Speed", ONE_VAL_SIZE);
    m_panel->addElement(m_particleSpeed);

    m_panel->addNewLine();
    auto deletePathButton{ std::make_shared<UIButton>("Delete Path", ONE_BUTTON_SIZE) };
    m_panel->addElement(deletePathButton);

    m_panel->addNewLine();
    auto oscillateLabel{ std::make_shared<UIText>("Emitter Oscillation", LABEL_SIZE) };
    m_panel->addElement(oscillateLabel);

    m_panel->addNewLine();
    auto hSinLabel{ std::make_shared<UIText>("Horizontal Sine", LABEL_SIZE) };
    m_panel->addElement(hSinLabel);

    m_panel->addNewHalfLine();
    m_hSineAmplitude = std::make_shared<UITextField>("Amplitude", TWO_VAL_SIZE);
    m_panel->addElement(m_hSineAmplitude);

    m_hSinePeriod = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_panel->addElement(m_hSinePeriod);

    m_panel->addNewLine();
    auto vSinLabel{ std::make_shared<UIText>("Vertical Sine", LABEL_SIZE) };
    m_panel->addElement(vSinLabel);

    m_panel->addNewHalfLine();
    m_vSineAmplitude = std::make_shared<UITextField>("Amplitude", TWO_VAL_SIZE);
    m_panel->addElement(m_vSineAmplitude);

    m_vSinePeriod = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_panel->addElement(m_vSinePeriod);

    m_panel->addNewLine();
    auto circleLabel{ std::make_shared<UIText>("Circle", LABEL_SIZE) };
    m_panel->addElement(circleLabel);

    m_panel->addNewHalfLine();
    m_circleRadius = std::make_shared<UITextField>("Radius", TWO_VAL_SIZE);
    m_panel->addElement(m_circleRadius);

    m_circlePeriod = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_panel->addElement(m_circlePeriod);

    m_panel->addToRenderer(uRenderer, tRenderer);
    m_panel->setEnabled(false);
}

void MovementPanel::update(float deltaTime, Emitter &emitter)
{
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
    m_hSineAmplitude->setValue(emitter.getHSineAmplitude());
    m_hSinePeriod->setValue(emitter.getHSinePeriod());

    m_vSineAmplitude->setValue(emitter.getVSineAmplitude());
    m_vSinePeriod->setValue(emitter.getVSinePeriod());

    m_circleRadius->setValue(emitter.getCircleRadius());
    m_circlePeriod->setValue(emitter.getCirclePeriod());
}

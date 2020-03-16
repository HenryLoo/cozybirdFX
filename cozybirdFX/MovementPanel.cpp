#include "MovementPanel.h"
#include "Emitter.h"
#include "UIButton.h"
#include "UIText.h"
#include "UITextField.h"

MovementPanel::MovementPanel(TextRenderer *tRenderer, UIRenderer *uRenderer)
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
    m_particleSpeedField = std::make_shared<UITextField>("Particle Speed", ONE_VAL_SIZE);
    m_panel->addElement(m_particleSpeedField);

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
    m_hSinAmpField = std::make_shared<UITextField>("Amplitude", TWO_VAL_SIZE);
    m_panel->addElement(m_hSinAmpField);

    m_hSinPeriodField = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_panel->addElement(m_hSinPeriodField);

    m_panel->addNewLine();
    auto vSinLabel{ std::make_shared<UIText>("Vertical Sine", LABEL_SIZE) };
    m_panel->addElement(vSinLabel);

    m_panel->addNewHalfLine();
    m_vSinAmpField = std::make_shared<UITextField>("Amplitude", TWO_VAL_SIZE);
    m_panel->addElement(m_vSinAmpField);

    m_vSinPeriodField = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_panel->addElement(m_vSinPeriodField);

    m_panel->addNewLine();
    auto circleLabel{ std::make_shared<UIText>("Circle", LABEL_SIZE) };
    m_panel->addElement(circleLabel);

    m_panel->addNewHalfLine();
    m_circleRadiusField = std::make_shared<UITextField>("Radius", TWO_VAL_SIZE);
    m_panel->addElement(m_circleRadiusField);

    m_circlePeriodField = std::make_shared<UITextField>("Period", TWO_VAL_SIZE);
    m_panel->addElement(m_circlePeriodField);

    m_panel->addToRenderer(uRenderer, tRenderer);
    m_panel->setEnabled(false);
}

void MovementPanel::update(Emitter *emitter, float deltaTime)
{
    // Set horizontal sine movement amplitude.
    float hSinAmp;
    bool isHSinAmp{ m_hSinAmpField->getValue(hSinAmp) };
    if (isHSinAmp)
    {
        emitter->setHSineAmplitude(hSinAmp);
    }

    // Set horizontal sine movement period.
    float hSinPeriod;
    bool isHSinPeriod{ m_hSinPeriodField->getValue(hSinPeriod) };
    if (isHSinPeriod)
    {
        emitter->setHSinePeriod(hSinPeriod);
    }

    // Set vertical sine movement amplitude.
    float vSinAmp;
    bool isVSinAmp{ m_vSinAmpField->getValue(vSinAmp) };
    if (isVSinAmp)
    {
        emitter->setVSineAmplitude(vSinAmp);
    }

    // Set vertical sine movement period.
    float vSinPeriod;
    bool isVSinPeriod{ m_vSinPeriodField->getValue(vSinPeriod) };
    if (isVSinPeriod)
    {
        emitter->setVSinePeriod(vSinPeriod);
    }

    // Set circle movement radius.
    float circleRadius;
    bool isCircleRadius{ m_circleRadiusField->getValue(circleRadius) };
    if (isCircleRadius)
    {
        emitter->setCircleRadius(circleRadius);
    }

    // Set circle movement period.
    float circlePeriod;
    bool isCirclePeriod{ m_circlePeriodField->getValue(circlePeriod) };
    if (isCirclePeriod)
    {
        emitter->setCirclePeriod(circlePeriod);
    }
}

void MovementPanel::updateUIFromEmitter(Emitter *emitter)
{
    m_hSinAmpField->setValue(emitter->getHSineAmplitude());
    m_hSinPeriodField->setValue(emitter->getHSinePeriod());

    m_vSinAmpField->setValue(emitter->getVSineAmplitude());
    m_vSinPeriodField->setValue(emitter->getVSinePeriod());

    m_circleRadiusField->setValue(emitter->getCircleRadius());
    m_circlePeriodField->setValue(emitter->getCirclePeriod());
}

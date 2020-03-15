#include "RenderPanel.h"
#include "EditorState.h"
#include "EmitterRenderer.h"
#include "UIButton.h"
#include "UIText.h"
#include "UITextField.h"

RenderPanel::RenderPanel(EditorState *editor, 
    std::shared_ptr<EmitterRenderer> eRenderer,
    TextRenderer *tRenderer, UIRenderer *uRenderer,
    UIRenderer::Properties *clipSizeBox) :
    m_editor(editor), m_eRenderer(eRenderer), m_clipSizeBox(clipSizeBox)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    auto clipLabel{ std::make_shared<UIText>("Clip Size", LABEL_SIZE) };
    m_panel->addElement(clipLabel);

    m_panel->addNewHalfLine();
    m_clipXField = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_panel->addElement(m_clipXField);

    m_clipYField = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_panel->addElement(m_clipYField);

    m_panel->addNewLine();
    m_durationField = std::make_shared<UITextField>("Animation Duration", ONE_VAL_SIZE);
    m_panel->addElement(m_durationField);

    m_panel->addNewLine();
    auto loopButton{ std::make_shared<UIButton>("Animation is Looping", ONE_BUTTON_SIZE, true) };
    loopButton->setAction([eRenderer, loopButton]()
        {
            eRenderer->setLooping(loopButton->isToggled());
        });
    m_panel->addElement(loopButton);

    m_panel->addNewLine();
    m_fpsField = std::make_shared<UITextField>("Export FPS", ONE_VAL_SIZE);
    m_panel->addElement(m_fpsField);

    m_panel->addToRenderer(uRenderer, tRenderer);
    m_panel->setEnabled(false);
    loopButton->setToggled(true);
}

void RenderPanel::update(Emitter *emitter, float deltaTime)
{
    // Set the clip size.
    glm::ivec2 clipSize;
    bool isClipX{ m_clipXField->getValue(clipSize.x) };
    bool isClipY{ m_clipYField->getValue(clipSize.y) };
    if (isClipX || isClipY)
    {
        m_clipSizeBox->size = clipSize;
        m_editor->updateClipBoxPos();
        m_eRenderer->setClipSize(clipSize);
    }

    // Set the animation duration.
    float duration;
    bool isDuration{ m_durationField->getValue(duration) };
    if (isDuration)
    {
        m_eRenderer->setDuration(duration);
    }

    // Set the export FPS.
    int fps;
    bool isFPS{ m_fpsField->getValue(fps) };
    if (isFPS)
    {
        m_eRenderer->setExportFPS(fps);
    }
}

void RenderPanel::updateUIFromEmitter(Emitter *emitter)
{
    glm::ivec2 clipSize{ m_eRenderer->getClipSize() };
    m_clipXField->setValue(clipSize.x);
    m_clipYField->setValue(clipSize.y);
    m_durationField->setValue(m_eRenderer->getDuration());
    m_fpsField->setValue(m_eRenderer->getExportFPS());
}

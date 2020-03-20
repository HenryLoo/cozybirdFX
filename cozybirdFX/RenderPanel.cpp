#include "RenderPanel.h"
#include "EditorState.h"
#include "EmitterRenderer.h"
#include "UIButton.h"
#include "UIText.h"
#include "UITextField.h"

RenderPanel::RenderPanel(EditorState &editor, 
    std::shared_ptr<EmitterRenderer> eRenderer,
    TextRenderer &tRenderer, UIRenderer &uRenderer,
    UIRenderer::Properties &clipSizeBox) :
    m_editor(editor), m_eRenderer(eRenderer), m_clipSizeBox(clipSizeBox)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    auto clipLabel{ std::make_shared<UIText>("Clip Size", LABEL_SIZE) };
    m_panel->addElement(clipLabel);

    m_panel->addNewHalfLine();
    m_clipX = std::make_shared<UITextField>("x", TWO_VAL_SIZE);
    m_panel->addElement(m_clipX);

    m_clipY = std::make_shared<UITextField>("y", TWO_VAL_SIZE);
    m_panel->addElement(m_clipY);

    m_panel->addNewLine();
    m_duration = std::make_shared<UITextField>("Animation Duration", ONE_VAL_SIZE);
    m_panel->addElement(m_duration);

    m_panel->addNewLine();
    m_loopButton = std::make_shared<UIButton>("Animation is Looping", ONE_BUTTON_SIZE, true);
    const UIButton &loopButton{ *m_loopButton };
    m_loopButton->setAction([eRenderer, &loopButton]()
        {
            eRenderer->setLooping(loopButton.isToggled());
        });
    m_panel->addElement(m_loopButton);

    m_panel->addNewLine();
    m_fps = std::make_shared<UITextField>("Export FPS", ONE_VAL_SIZE);
    m_panel->addElement(m_fps);

    m_panel->addToRenderer(uRenderer, tRenderer);
    m_panel->setEnabled(false);
}

void RenderPanel::update(float deltaTime, Emitter &emitter)
{
    // Set the clip size.
    glm::ivec2 clipSize;
    bool isClipX{ m_clipX->getValue(clipSize.x) };
    bool isClipY{ m_clipY->getValue(clipSize.y) };
    if (isClipX || isClipY)
    {
        m_clipSizeBox.size = clipSize;
        m_editor.updateClipBoxPos();
        m_eRenderer->setClipSize(clipSize);
    }

    // Set the animation duration.
    float duration;
    bool isDuration{ m_duration->getValue(duration) };
    if (isDuration)
    {
        m_eRenderer->setDuration(duration);
    }

    // Set the export FPS.
    int fps;
    bool isFPS{ m_fps->getValue(fps) };
    if (isFPS)
    {
        m_eRenderer->setExportFPS(fps);
    }
}

void RenderPanel::updateUIFromEmitter(const Emitter &emitter)
{
    glm::ivec2 clipSize{ m_eRenderer->getClipSize() };
    m_clipX->setValue(clipSize.x);
    m_clipY->setValue(clipSize.y);
    m_duration->setValue(m_eRenderer->getDuration());
    m_fps->setValue(m_eRenderer->getExportFPS());
    m_loopButton->setToggled(m_eRenderer->isLooping());
}

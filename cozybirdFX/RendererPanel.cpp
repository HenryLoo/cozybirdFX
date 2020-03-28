#include "RendererPanel.h"
#include "EditorState.h"
#include "EmitterRenderer.h"
#include "UIButton.h"
#include "UIText.h"
#include "UIIntField.h"

RendererPanel::RendererPanel(EditorState &editor, 
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
    m_clipX = std::make_shared<UIIntField>("Width", TWO_VAL_SIZE);
    m_clipX->setDescription("The width of each frame in the animation. Only details inside the clip will be considered when exporting.");
    m_panel->addElement(m_clipX);

    m_clipY = std::make_shared<UIIntField>("Height", TWO_VAL_SIZE);
    m_clipY->setDescription("The height of each frame in the animation. Only details inside the clip will be considered when exporting.");
    m_panel->addElement(m_clipY);

    m_panel->addNewLine();
    m_loop = std::make_shared<UIButton>("Animation is Looping", ONE_BUTTON_SIZE, true);
    m_loop->setDescription("Set whether the animation is looping or not.");
    const UIButton &loopButton{ *m_loop };
    m_loop->setAction([eRenderer, &loopButton]()
        {
            eRenderer->setLooping(loopButton.isToggled());
        });
    m_panel->addElement(m_loop);

    m_panel->addNewLine();
    m_fps = std::make_shared<UIIntField>("Export FPS", ONE_VAL_SIZE);
    m_fps->setDescription("The frame rate of the exported animation. Setting this too low may fail to capture some emitter details.");
    m_panel->addElement(m_fps);

    m_panel->addToRenderer(uRenderer, tRenderer);
    m_panel->setEnabled(false);
}

void RendererPanel::update(float deltaTime, Emitter &emitter)
{
    // Set the clip size.
    glm::ivec2 clipSize;
    bool isClipX{ m_clipX->getValue(clipSize.x) };
    bool isClipY{ m_clipY->getValue(clipSize.y) };
    if (isClipX || isClipY)
    {
        m_eRenderer->setClipSize(clipSize);
        m_editor.updateClipBoxPos();
    }

    // Set the export FPS.
    int fps;
    bool isFPS{ m_fps->getValue(fps) };
    if (isFPS)
    {
        m_eRenderer->setExportFPS(fps);
    }
}

void RendererPanel::updateUIFromEmitter(const Emitter &emitter)
{
    glm::ivec2 clipSize{ m_eRenderer->getClipSize() };
    m_clipX->setValue(clipSize.x);
    m_clipY->setValue(clipSize.y);
    m_fps->setValue(m_eRenderer->getExportFPS());
    m_loop->setToggled(m_eRenderer->isLooping());
}

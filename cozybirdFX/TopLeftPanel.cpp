#include "TopLeftPanel.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "UIButton.h"
#include "UIRenderer.h"

#include <iomanip>
#include <sstream>

TopLeftPanel::TopLeftPanel(Engine *engine, 
    std::shared_ptr<EmitterRenderer> eRenderer, TextRenderer *tRenderer,
    UIRenderer *uRenderer, UIRenderer::Properties *clipSizeBox) :
    m_eRenderer(eRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, -1.f));

    auto fileButton{ std::make_shared<UIButton>("File",
        BUTTON_SIZE, false, []() {  }) };
    m_panel->addElement(fileButton);

    auto exportButton{ std::make_shared<UIButton>("Export",
        BUTTON_SIZE, false,
        [eRenderer, engine]()
        {
            glm::ivec2 windowSize{ engine->getWindowSize() };
            eRenderer->exportSpriteSheet(windowSize);
        }) };
    m_panel->addElement(exportButton);

    UIRenderer::Properties *clipSize{ clipSizeBox };
    auto clipButton{ std::make_shared<UIButton>("Show Clip",
       BUTTON_SIZE, true,
       [clipSize]() { clipSize->isEnabled = !clipSize->isEnabled; }) };
    m_panel->addElement(clipButton);

    auto playButton{ std::make_shared<UIButton>("Play", BUTTON_SIZE, true) };
    playButton->setAction([eRenderer, playButton]()
        {
            eRenderer->setPlaying(playButton->isToggled());
        });
    m_panel->addElement(playButton);

    m_playbackTimer = std::make_shared<UIText>("", BUTTON_SIZE);
    m_panel->addElement(m_playbackTimer);

    m_panel->addToRenderer(uRenderer, tRenderer);
    playButton->setToggled(true);
    clipButton->setToggled(true);
}

void TopLeftPanel::update(Emitter *emitter, float deltaTime)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3);

    float currentTime{ m_eRenderer->getCurrentTime() };
    float duration{ m_eRenderer->getDuration() };
    ss << "[" << currentTime << " / " << duration << "]";

    m_playbackTimer->setText(ss.str());
}

void TopLeftPanel::updateUIFromEmitter(Emitter *emitter)
{

}
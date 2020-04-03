#include "TopLeftPanel.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "ExportState.h"
#include "FileState.h"
#include "UIButton.h"
#include "UIRenderer.h"
#include "UIText.h"

#include <iomanip>
#include <sstream>

TopLeftPanel::TopLeftPanel(Engine &engine, AssetLoader &assetLoader, 
    std::shared_ptr<EmitterRenderer> eRenderer, 
    SpriteRenderer &sRenderer, TextRenderer &tRenderer, UIRenderer &uRenderer,
    UIRenderer::Properties &clipSizeBox) :
    m_eRenderer(eRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(0.f, -1.f));

    auto fileButton{ std::make_shared<UIButton>("File",
        BUTTON_SIZE, false, [&engine, &assetLoader, eRenderer]()
        {  
            FileState *state{ new FileState(engine, assetLoader, *eRenderer) };
            engine.pushState(state);
        }) };
    m_panel->addElement(fileButton);

    auto exportButton{ std::make_shared<UIButton>("Export",
        BUTTON_SIZE, false,
        [&engine, &assetLoader, eRenderer]()
        {
            ExportState *state{ new ExportState(engine, assetLoader, *eRenderer) };
            engine.pushState(state);
        }) };
    m_panel->addElement(exportButton);

    auto clipButton{ std::make_shared<UIButton>("Show Clip",
       BUTTON_SIZE, true,
       [&clipSizeBox]() { clipSizeBox.isEnabled = !clipSizeBox.isEnabled; }) };
    m_panel->addElement(clipButton);

    m_playButton = std::make_shared<UIButton>("Play", BUTTON_SIZE, true);
    auto playButton{ m_playButton };
    m_playButton->setAction([eRenderer, playButton]()
        {
            eRenderer->setPlaying(playButton->isToggled());
        });
    m_panel->addElement(playButton);

    m_playbackTimer = std::make_shared<UIText>("", BUTTON_SIZE);
    m_panel->addElement(m_playbackTimer);

    m_panel->addToRenderer(sRenderer, uRenderer, tRenderer);
    clipButton->setToggled(true);
}

void TopLeftPanel::update(float deltaTime, Emitter &emitter)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3);

    float currentTime{ m_eRenderer->getCurrentTime() };
    float duration{ m_eRenderer->getDuration() };
    ss << "[" << currentTime << " / " << duration << "]";

    m_playbackTimer->setText(ss.str());
}

void TopLeftPanel::updateUIFromEmitter(const Emitter &emitter)
{
    m_playButton->setToggled(m_eRenderer->isPlaying());
}

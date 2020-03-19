#include "EmittersPanel.h"
#include "EditorState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "UIButton.h"
#include "UIText.h"

EmittersPanel::EmittersPanel(EditorState &editor, 
    EmitterRenderer &eRenderer, TextRenderer &tRenderer, 
    UIRenderer &uRenderer)
{
    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    auto emittersLabel{ std::make_shared<UIText>("Select/Toggle Emitters", LABEL_SIZE) };
    m_panel->addElement(emittersLabel);

    m_panel->addNewLine();
    std::vector<std::shared_ptr<UIButton>> buttons;
    for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
    {
        auto emSelectButton{ std::make_shared<UIButton>("Emitter " + std::to_string(i + 1),
            TWO_BUTTON_SIZE, true) };
        buttons.push_back(emSelectButton);
    }

    std::shared_ptr<UIButton> firstToggle{ nullptr };
    for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
    {
        auto &button{ buttons[i] };
        button->setAction([&editor, i, buttons]()
            {
                editor.selectEmitter(i);

                for (int j = 0; j < buttons.size(); ++j)
                {
                    bool val{ j == i };
                    buttons[j]->setToggled(val);
                }
            });
        m_panel->addElement(button);

        auto emToggleButton{ std::make_shared<UIButton>("ON", TWO_BUTTON_SIZE, true) };
        emToggleButton->setAction([&eRenderer, emToggleButton, i]()
            {
                eRenderer.toggleEmitter(i, emToggleButton->isToggled());
            });
        m_panel->addElement(emToggleButton);

        if (i == 0)
            firstToggle = emToggleButton;

        if (i < EmitterRenderer::NUM_EMITTERS - 1)
            m_panel->addNewLine();
    }

    m_panel->addToRenderer(uRenderer, tRenderer);
    buttons[0]->setToggled(true);
    firstToggle->setToggled(true);
    m_panel->setEnabled(false);
}

void EmittersPanel::update(float deltaTime, Emitter &emitter)
{
}

void EmittersPanel::updateUIFromEmitter(const Emitter &emitter)
{
}

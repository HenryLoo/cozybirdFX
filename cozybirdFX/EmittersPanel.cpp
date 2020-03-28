#include "EmittersPanel.h"
#include "EditorState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "UIButton.h"
#include "UIText.h"

#include <GLFW/glfw3.h>

namespace
{
    const std::vector<int> EMITTER_HOTKEYS{
        GLFW_KEY_F1,
        GLFW_KEY_F2,
        GLFW_KEY_F3,
        GLFW_KEY_F4,
        GLFW_KEY_F5,
        GLFW_KEY_F6,
        GLFW_KEY_F7,
        GLFW_KEY_F8,
        GLFW_KEY_F9,
    };
}

EmittersPanel::EmittersPanel(EditorState &editor, 
    std::shared_ptr<EmitterRenderer> eRenderer, TextRenderer &tRenderer,
    UIRenderer &uRenderer) :
    m_editor(editor), m_eRenderer(eRenderer)
{
    m_description = "Press F1 ~ F9 at any panel to quickly select emitters 1 ~ 9 respectively.";

    m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
        glm::vec2(-1.f, 0.f));

    auto emittersLabel{ std::make_shared<UIText>("Select/Toggle Emitters", LABEL_SIZE) };
    m_panel->addElement(emittersLabel);

    m_panel->addNewLine();
    for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
    {
        const std::string emitterNum{ std::to_string(i + 1) };
        auto emSelectButton{ std::make_shared<UIButton>("Emitter " + emitterNum,
            TWO_BUTTON_SIZE, true) };
        emSelectButton->setDescription("Select emitter " + emitterNum + " as the current emitter.");
        m_selectButtons.push_back(emSelectButton);
    }

    for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
    {
        auto &button{ m_selectButtons[i] };
        button->setAction([this, i]()
            {
                this->selectEmitter(i);
            });
        m_panel->addElement(button);

        auto emToggleButton{ std::make_shared<UIButton>("ON", TWO_BUTTON_SIZE, true) };
        emToggleButton->setAction([eRenderer, emToggleButton, i]()
            {
                eRenderer->toggleEmitter(i, emToggleButton->isToggled());
            });
        emToggleButton->setDescription("Show/hide emitter " + std::to_string(i + 1) + ".");
        m_panel->addElement(emToggleButton);
        m_toggleButtons.push_back(emToggleButton);

        if (i < EmitterRenderer::NUM_EMITTERS - 1)
            m_panel->addNewLine();
    }

    m_panel->addToRenderer(uRenderer, tRenderer);
    m_selectButtons[0]->setToggled(true);
    m_panel->setEnabled(false);
}

void EmittersPanel::handleInput(InputManager &inputManager)
{
    IEditorPanel::handleInput(inputManager);

    for (int i = 0; i < EMITTER_HOTKEYS.size(); ++i)
    {
        if (inputManager.isKeyDown(EMITTER_HOTKEYS[i]))
        {
            selectEmitter(i);
            return;
        }
    }
}

void EmittersPanel::update(float deltaTime, Emitter &emitter)
{
}

void EmittersPanel::updateUIFromEmitter(const Emitter &emitter)
{
    for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
    {
        m_toggleButtons[i]->setToggled(m_eRenderer->isEnabled(i));
    }
}

void EmittersPanel::selectEmitter(int index) const
{
    m_editor.selectEmitter(index);

    for (int j = 0; j < m_selectButtons.size(); ++j)
    {
        bool val{ j == index };
        m_selectButtons[j]->setToggled(val);
    }
}

#pragma once

#include "IEditorPanel.h"

class EditorState;

class EmitterRenderer;
class SpriteRenderer;
class TextRenderer;
class UIButton;
class UIRenderer;

class EmittersPanel : public IEditorPanel
{
public:
	EmittersPanel(EditorState &editor, 
		std::shared_ptr<EmitterRenderer> eRenderer, 
		SpriteRenderer &sRenderer, TextRenderer &tRenderer, 
		UIRenderer &uRenderer);

	virtual bool handleInput(InputManager &inputManager,
		UndoableAction &action) override;

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

	void selectEmitter(int index) const;

private:
	EditorState &m_editor;
	std::shared_ptr<EmitterRenderer> m_eRenderer{ nullptr };

	std::vector<std::shared_ptr<UIButton>> m_selectButtons;
	std::vector<std::shared_ptr<UIButton>> m_toggleButtons;
};
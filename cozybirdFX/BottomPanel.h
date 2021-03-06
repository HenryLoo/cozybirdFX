#pragma once

#include "IEditorPanel.h"

class EditorState;
class SpriteRenderer;
class TextRenderer;
class UIRenderer;
class UIText;

class ParticlesPanel;
class VisualsPanel;
class MovementPanel;
class EmittersPanel;
class RendererPanel;

class BottomPanel : public IEditorPanel
{
public:
	BottomPanel(EditorState &editor, SpriteRenderer &sRenderer,
		TextRenderer &tRenderer, UIRenderer &uRenderer,
		std::shared_ptr<ParticlesPanel> particles,
		std::shared_ptr<VisualsPanel> visuals,
		std::shared_ptr<MovementPanel> movement,
		std::shared_ptr<EmittersPanel> emitters,
		std::shared_ptr<RendererPanel> renderer);

	virtual bool handleInput(InputManager &inputManager,
		UndoableAction &action) override;

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

public:
	EditorState &m_editor;
	std::shared_ptr<UIText> m_text{ nullptr };
};
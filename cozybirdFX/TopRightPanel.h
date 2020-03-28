#pragma once

#include "IEditorPanel.h"

class EditorState;
class TextRenderer;
class UIButton;
class UIRenderer;

class ParticlesPanel;
class VisualsPanel;
class MovementPanel;
class EmittersPanel;
class RendererPanel;

class TopRightPanel : public IEditorPanel
{
public:
	TopRightPanel(EditorState &state, TextRenderer &tRenderer, 
		UIRenderer &uRenderer, 
		std::shared_ptr<ParticlesPanel> particles, 
		std::shared_ptr<VisualsPanel> visuals, 
		std::shared_ptr<MovementPanel> movement,
		std::shared_ptr<EmittersPanel> emitters, 
		std::shared_ptr<RendererPanel> renderer);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

private:
	void setCurrentButton(std::shared_ptr<UIButton> button);

	std::shared_ptr<UIButton> m_currentButton{ nullptr };

	std::shared_ptr<UIButton> m_particlesButton{ nullptr };
	std::shared_ptr<UIButton> m_visualsButton{ nullptr };
	std::shared_ptr<UIButton> m_movementButton{ nullptr };
	std::shared_ptr<UIButton> m_emittersButton{ nullptr };
	std::shared_ptr<UIButton> m_rendererButton{ nullptr };
};
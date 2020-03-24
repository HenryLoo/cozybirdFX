#pragma once

#include "IEditorPanel.h"
#include "UIRenderer.h"

class EditorState;
class EmitterRenderer;
class TextRenderer;
class UIButton;
class UITextField;

class RendererPanel : public IEditorPanel
{
public:
	RendererPanel(EditorState &editor,
		std::shared_ptr<EmitterRenderer> eRenderer,
		TextRenderer &tRenderer, UIRenderer &uRenderer,
		UIRenderer::Properties &clipSizeBox);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

private:
	EditorState &m_editor;
	std::shared_ptr<EmitterRenderer> m_eRenderer{ nullptr };
	UIRenderer::Properties &m_clipSizeBox;

	std::shared_ptr<UITextField> m_clipX{ nullptr };
	std::shared_ptr<UITextField> m_clipY{ nullptr };
	std::shared_ptr<UITextField> m_duration{ nullptr };
	std::shared_ptr<UITextField> m_fps{ nullptr };
	std::shared_ptr<UIButton> m_loop{ nullptr };
};
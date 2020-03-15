#pragma once

#include "IEditorPanel.h"
#include "UIRenderer.h"

class EditorState;
class EmitterRenderer;
class TextRenderer;
class UITextField;

class RenderPanel : public IEditorPanel
{
public:
	RenderPanel(EditorState *editor,
		std::shared_ptr<EmitterRenderer> eRenderer,
		TextRenderer *tRenderer, UIRenderer *uRenderer,
		UIRenderer::Properties *clipSizeBox);

	virtual void update(Emitter *emitter, float deltaTime) override;

	virtual void updateUIFromEmitter(Emitter *emitter) override;

private:
	EditorState *m_editor{ nullptr };
	std::shared_ptr<EmitterRenderer> m_eRenderer{ nullptr };
	UIRenderer::Properties *m_clipSizeBox{ nullptr };

	std::shared_ptr<UITextField> m_clipXField{ nullptr };
	std::shared_ptr<UITextField> m_clipYField{ nullptr };
	std::shared_ptr<UITextField> m_durationField{ nullptr };
	std::shared_ptr<UITextField> m_fpsField{ nullptr };
};
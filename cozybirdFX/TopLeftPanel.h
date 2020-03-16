#pragma once

#include "IEditorPanel.h"
#include "UIText.h"

class Engine;
class EmitterRenderer;
class TextRenderer;
class UIRenderer;

class TopLeftPanel : public IEditorPanel
{
public:
	TopLeftPanel(Engine *engine, std::shared_ptr<EmitterRenderer> eRenderer,
		TextRenderer *tRenderer, UIRenderer *uRenderer,
		UIRenderer::Properties *clipSizeBox);

	virtual void update(Emitter *emitter, float deltaTime) override;

	virtual void updateUIFromEmitter(Emitter *emitter) override;

private:
	std::shared_ptr<EmitterRenderer> m_eRenderer{ nullptr };
	std::shared_ptr<UIText> m_playbackTimer{ nullptr };
};
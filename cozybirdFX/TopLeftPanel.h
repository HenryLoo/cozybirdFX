#pragma once

#include "IEditorPanel.h"

class AssetLoader;
class Engine;
class EmitterRenderer;
class TextRenderer;
class UIButton;
class UIText;
class UIRenderer;

class TopLeftPanel : public IEditorPanel
{
public:
	TopLeftPanel(Engine &engine, AssetLoader &assetLoader,
		std::shared_ptr<EmitterRenderer> eRenderer,
		TextRenderer &tRenderer, UIRenderer &uRenderer,
		UIRenderer::Properties &clipSizeBox);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

private:
	std::shared_ptr<EmitterRenderer> m_eRenderer{ nullptr };
	std::shared_ptr<UIText> m_playbackTimer{ nullptr };
	std::shared_ptr<UIButton> m_playButton{ nullptr };
};
#pragma once

#include "IEditorPanel.h"
#include "Emitter.h"

class AssetLoader;
class Texture;
class TextRenderer;
class UIButton;
class UIRenderer;
class UISlider;
class UITextField;

class VisualsPanel : public IEditorPanel
{
public:
	VisualsPanel(TextRenderer &tRenderer, UIRenderer &uRenderer, 
		AssetLoader &assetLoader);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

	void prepareToLoadTexture();

private:
	std::shared_ptr<UIButton> m_linearBlend{ nullptr };
	std::shared_ptr<UIButton> m_additiveBlend{ nullptr };
	std::shared_ptr<UISlider> m_red{ nullptr };
	std::shared_ptr<UISlider> m_green{ nullptr };
	std::shared_ptr<UISlider> m_blue{ nullptr };
	std::shared_ptr<UISlider> m_opacity{ nullptr };
	std::shared_ptr<UISlider> m_birthRed{ nullptr };
	std::shared_ptr<UISlider> m_birthGreen{ nullptr };
	std::shared_ptr<UISlider> m_birthBlue{ nullptr };
	std::shared_ptr<UISlider> m_birthOpacity{ nullptr };
	std::shared_ptr<UISlider> m_deathRed{ nullptr };
	std::shared_ptr<UISlider> m_deathGreen{ nullptr };
	std::shared_ptr<UISlider> m_deathBlue{ nullptr };
	std::shared_ptr<UISlider> m_deathOpacity{ nullptr };

	std::shared_ptr<UITextField> m_textureName{ nullptr };
	bool m_hasNewTexture{ true };
	AssetLoader &m_assetLoader;

	Emitter::BlendMode m_blendMode{ Emitter::BlendMode::Linear };
};
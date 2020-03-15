#pragma once

#include "IEditorPanel.h"

class TextRenderer;
class UIRenderer;
class UISlider;

class VisualsPanel : public IEditorPanel
{
public:
	VisualsPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);

	virtual void update(Emitter *emitter, float deltaTime) override;

	virtual void updateUIFromEmitter(Emitter *emitter) override;

private:
	std::shared_ptr<UISlider> m_rSlider{ nullptr };
	std::shared_ptr<UISlider> m_gSlider{ nullptr };
	std::shared_ptr<UISlider> m_bSlider{ nullptr };
	std::shared_ptr<UISlider> m_aSlider{ nullptr };
	std::shared_ptr<UISlider> m_birthRSlider{ nullptr };
	std::shared_ptr<UISlider> m_birthGSlider{ nullptr };
	std::shared_ptr<UISlider> m_birthBSlider{ nullptr };
	std::shared_ptr<UISlider> m_birthASlider{ nullptr };
	std::shared_ptr<UISlider> m_deathRSlider{ nullptr };
	std::shared_ptr<UISlider> m_deathGSlider{ nullptr };
	std::shared_ptr<UISlider> m_deathBSlider{ nullptr };
	std::shared_ptr<UISlider> m_deathASlider{ nullptr };
};
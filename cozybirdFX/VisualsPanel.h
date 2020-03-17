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
	std::shared_ptr<UISlider> m_red{ nullptr };
	std::shared_ptr<UISlider> m_green{ nullptr };
	std::shared_ptr<UISlider> m_blue{ nullptr };
	std::shared_ptr<UISlider> m_opacity{ nullptr };
	std::shared_ptr<UISlider> m_additivity{ nullptr };
	std::shared_ptr<UISlider> m_birthRed{ nullptr };
	std::shared_ptr<UISlider> m_birthGreen{ nullptr };
	std::shared_ptr<UISlider> m_birthBlue{ nullptr };
	std::shared_ptr<UISlider> m_birthOpacity{ nullptr };
	std::shared_ptr<UISlider> m_birthAdditivity{ nullptr };
	std::shared_ptr<UISlider> m_deathRed{ nullptr };
	std::shared_ptr<UISlider> m_deathGreen{ nullptr };
	std::shared_ptr<UISlider> m_deathBlue{ nullptr };
	std::shared_ptr<UISlider> m_deathOpacity{ nullptr };
	std::shared_ptr<UISlider> m_deathAdditivity{ nullptr };
};
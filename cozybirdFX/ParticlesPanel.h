#pragma once

#include "IEditorPanel.h"

class TextRenderer;
class UIButton;
class UIRenderer;
class UISlider;
class UITextField;

class ParticlesPanel : public IEditorPanel
{
public:
	ParticlesPanel(TextRenderer &tRenderer, UIRenderer &uRenderer);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

private:
	std::shared_ptr<UITextField> m_delay{ nullptr };
	std::shared_ptr<UITextField> m_duration{ nullptr };
	std::shared_ptr<UITextField> m_xPosition{ nullptr };
	std::shared_ptr<UITextField> m_yPosition{ nullptr };
	std::shared_ptr<UITextField> m_spawnTime{ nullptr };
	std::shared_ptr<UITextField> m_distWidth{ nullptr };
	std::shared_ptr<UITextField> m_distHeight{ nullptr };
	std::shared_ptr<UITextField> m_numToGenerate{ nullptr };
	std::shared_ptr<UIButton> m_facingDir{ nullptr };
	std::shared_ptr<UISlider> m_rotationMin{ nullptr };
	std::shared_ptr<UISlider> m_rotationMax{ nullptr };
	std::shared_ptr<UISlider> m_rotationGrowth{ nullptr };
	std::shared_ptr<UITextField> m_sizeMin{ nullptr };
	std::shared_ptr<UITextField> m_sizeMax{ nullptr };
	std::shared_ptr<UITextField> m_sizeGrowth{ nullptr };
	std::shared_ptr<UITextField> m_lifeMin{ nullptr };
	std::shared_ptr<UITextField> m_lifeMax{ nullptr };
};
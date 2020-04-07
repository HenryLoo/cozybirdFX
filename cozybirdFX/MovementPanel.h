#pragma once

#include "IEditorPanel.h"

class SpriteRenderer;
class TextRenderer;
class UIRenderer;
class UISlider;
class UIFloatField;
class UIIntField;

class MovementPanel : public IEditorPanel
{
public:
	MovementPanel(SpriteRenderer &sRenderer, TextRenderer &tRenderer, 
		UIRenderer &uRenderer);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

private:
	std::shared_ptr<UIFloatField> m_speedMin{ nullptr };
	std::shared_ptr<UIFloatField> m_speedMax{ nullptr };
	std::shared_ptr<UIFloatField> m_speedGrowth{ nullptr };
	std::shared_ptr<UISlider> m_directionMin{ nullptr };
	std::shared_ptr<UISlider> m_directionMax{ nullptr };
	std::shared_ptr<UIIntField> m_directionGrowth{ nullptr };
	std::shared_ptr<UIFloatField> m_hSineAmplitude{ nullptr };
	std::shared_ptr<UIFloatField> m_hSinePeriod{ nullptr };
	std::shared_ptr<UIFloatField> m_vSineAmplitude{ nullptr };
	std::shared_ptr<UIFloatField> m_vSinePeriod{ nullptr };
	std::shared_ptr<UIFloatField> m_circleRadius{ nullptr };
	std::shared_ptr<UIFloatField> m_circlePeriod{ nullptr };
};
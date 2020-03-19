#pragma once

#include "IEditorPanel.h"

class TextRenderer;
class UIRenderer;
class UITextField;

class MovementPanel : public IEditorPanel
{
public:
	MovementPanel(TextRenderer &tRenderer, UIRenderer &uRenderer);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

private:
	std::shared_ptr<UITextField> m_particleSpeed{ nullptr };
	std::shared_ptr<UITextField> m_hSineAmplitude{ nullptr };
	std::shared_ptr<UITextField> m_hSinePeriod{ nullptr };
	std::shared_ptr<UITextField> m_vSineAmplitude{ nullptr };
	std::shared_ptr<UITextField> m_vSinePeriod{ nullptr };
	std::shared_ptr<UITextField> m_circleRadius{ nullptr };
	std::shared_ptr<UITextField> m_circlePeriod{ nullptr };
};
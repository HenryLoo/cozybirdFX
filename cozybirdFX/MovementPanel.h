#pragma once

#include "IEditorPanel.h"

class TextRenderer;
class UIRenderer;
class UITextField;

class MovementPanel : public IEditorPanel
{
public:
	MovementPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);

	virtual void update(Emitter *emitter, float deltaTime) override;

	virtual void updateUIFromEmitter(Emitter *emitter) override;

private:
	std::shared_ptr<UITextField> m_particleSpeedField{ nullptr };
	std::shared_ptr<UITextField> m_hSinAmountField{ nullptr };
	std::shared_ptr<UITextField> m_hSinPeriodField{ nullptr };
	std::shared_ptr<UITextField> m_vSinAmountField{ nullptr };
	std::shared_ptr<UITextField> m_vSinPeriodField{ nullptr };
	std::shared_ptr<UITextField> m_circleRadiusField{ nullptr };
	std::shared_ptr<UITextField> m_circlePeriodField{ nullptr };
};
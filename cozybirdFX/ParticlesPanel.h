#pragma once

#include "IEditorPanel.h"

class TextRenderer;
class UIRenderer;
class UITextField;

class ParticlesPanel : public IEditorPanel
{
public:
	ParticlesPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);

	virtual void update(Emitter *emitter, float deltaTime) override;

	virtual void updateUIFromEmitter(Emitter *emitter) override;

private:
	std::shared_ptr<UITextField> m_delayField{ nullptr };
	std::shared_ptr<UITextField> m_emDurationField{ nullptr };
	std::shared_ptr<UITextField> m_xField{ nullptr };
	std::shared_ptr<UITextField> m_yField{ nullptr };
	std::shared_ptr<UITextField> m_numGenField{ nullptr };
	std::shared_ptr<UITextField> m_spawnTimeField{ nullptr };
	std::shared_ptr<UITextField> m_velXMinField{ nullptr };
	std::shared_ptr<UITextField> m_velXMaxField{ nullptr };
	std::shared_ptr<UITextField> m_velYMinField{ nullptr };
	std::shared_ptr<UITextField> m_velYMaxField{ nullptr };
	std::shared_ptr<UITextField> m_accelXField{ nullptr };
	std::shared_ptr<UITextField> m_accelYField{ nullptr };
	std::shared_ptr<UITextField> m_sizeField{ nullptr };
	std::shared_ptr<UITextField> m_lifeMinField{ nullptr };
	std::shared_ptr<UITextField> m_lifeMaxField{ nullptr };
};
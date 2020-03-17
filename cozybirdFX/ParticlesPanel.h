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
	std::shared_ptr<UITextField> m_delay{ nullptr };
	std::shared_ptr<UITextField> m_duration{ nullptr };
	std::shared_ptr<UITextField> m_xPosition{ nullptr };
	std::shared_ptr<UITextField> m_yPosition{ nullptr };
	std::shared_ptr<UITextField> m_numToGenerate{ nullptr };
	std::shared_ptr<UITextField> m_spawnTime{ nullptr };
	std::shared_ptr<UITextField> m_velXMin{ nullptr };
	std::shared_ptr<UITextField> m_velXMax{ nullptr };
	std::shared_ptr<UITextField> m_velYMin{ nullptr };
	std::shared_ptr<UITextField> m_velYMax{ nullptr };
	std::shared_ptr<UITextField> m_accelX{ nullptr };
	std::shared_ptr<UITextField> m_accelY{ nullptr };
	std::shared_ptr<UITextField> m_size{ nullptr };
	std::shared_ptr<UITextField> m_lifeMin{ nullptr };
	std::shared_ptr<UITextField> m_lifeMax{ nullptr };
};
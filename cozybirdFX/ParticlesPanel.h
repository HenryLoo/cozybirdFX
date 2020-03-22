#pragma once

#include "IEditorPanel.h"

class TextRenderer;
class UIRenderer;
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
	std::shared_ptr<UITextField> m_numToGenerate{ nullptr };
	std::shared_ptr<UITextField> m_spawnTime{ nullptr };
	std::shared_ptr<UITextField> m_speedMin{ nullptr };
	std::shared_ptr<UITextField> m_speedMax{ nullptr };
	std::shared_ptr<UITextField> m_speedGrowth{ nullptr };
	std::shared_ptr<UITextField> m_directionMin{ nullptr };
	std::shared_ptr<UITextField> m_directionMax{ nullptr };
	std::shared_ptr<UITextField> m_directionGrowth{ nullptr };
	std::shared_ptr<UITextField> m_rotationMin{ nullptr };
	std::shared_ptr<UITextField> m_rotationMax{ nullptr };
	std::shared_ptr<UITextField> m_rotationGrowth{ nullptr };
	std::shared_ptr<UITextField> m_sizeMin{ nullptr };
	std::shared_ptr<UITextField> m_sizeMax{ nullptr };
	std::shared_ptr<UITextField> m_sizeGrowth{ nullptr };
	std::shared_ptr<UITextField> m_lifeMin{ nullptr };
	std::shared_ptr<UITextField> m_lifeMax{ nullptr };
};
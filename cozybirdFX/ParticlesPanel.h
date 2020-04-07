#pragma once

#include "IEditorPanel.h"

class EditorState;
class SpriteRenderer;
class TextRenderer;
class UIButton;
class UIRenderer;
class UISlider;
class UIIntField;
class UIFloatField;

class ParticlesPanel : public IEditorPanel
{
public:
	ParticlesPanel(const EditorState &state, SpriteRenderer &sRenderer, 
		TextRenderer &tRenderer, UIRenderer &uRenderer);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;

private:
	const EditorState &m_state;

	std::shared_ptr<UIFloatField> m_delay{ nullptr };
	std::shared_ptr<UIFloatField> m_duration{ nullptr };
	std::shared_ptr<UIFloatField> m_xPosition{ nullptr };
	std::shared_ptr<UIFloatField> m_yPosition{ nullptr };
	std::shared_ptr<UIFloatField> m_spawnTime{ nullptr };
	std::shared_ptr<UIFloatField> m_distWidth{ nullptr };
	std::shared_ptr<UIFloatField> m_distHeight{ nullptr };
	std::shared_ptr<UIIntField> m_numToGenerate{ nullptr };
	std::shared_ptr<UIButton> m_facingDir{ nullptr };
	std::shared_ptr<UISlider> m_rotationMin{ nullptr };
	std::shared_ptr<UISlider> m_rotationMax{ nullptr };
	std::shared_ptr<UIIntField> m_rotationGrowth{ nullptr };
	std::shared_ptr<UIFloatField> m_sizeMin{ nullptr };
	std::shared_ptr<UIFloatField> m_sizeMax{ nullptr };
	std::shared_ptr<UIFloatField> m_sizeGrowth{ nullptr };
	std::shared_ptr<UIFloatField> m_lifeMin{ nullptr };
	std::shared_ptr<UIFloatField> m_lifeMax{ nullptr };
};
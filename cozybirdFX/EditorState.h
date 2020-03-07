#pragma once

#include "IState.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Engine;
class TextRenderer;
class UIRenderer;
class IUserInterface;
class UIContainer;
class UISlider;
class UITextField;

class EditorState : public IState
{
public:
	EditorState(Engine *engine, TextRenderer *tRenderer, UIRenderer *uRenderer);

	virtual void handleInput(InputManager *inputManager);

	virtual void update(Engine *engine, float deltaTime);

private:
	// Store the current window size.
	// Use this to detect window size changes and then rescale UI.
	glm::vec2 m_windowSize{ 0.f };

	// UI Elements for editing emitters.
	std::vector<std::shared_ptr<IUserInterface>> m_uiElements;
	std::shared_ptr<UIContainer> m_topLeftPanel;
	std::shared_ptr<UIContainer> m_topRightPanel;
	std::shared_ptr<UIContainer> m_bottomPanel;

	// Particles panel elements.
	std::shared_ptr<UIContainer> m_particlesPanel;
	std::shared_ptr<UITextField> m_xField;
	std::shared_ptr<UITextField> m_yField;
	std::shared_ptr<UITextField> m_numGenField;
	std::shared_ptr<UITextField> m_spawnTimeField;
	std::shared_ptr<UITextField> m_delayField;
	std::shared_ptr<UITextField> m_velXMinField;
	std::shared_ptr<UITextField> m_velXMaxField;
	std::shared_ptr<UITextField> m_velYMinField;
	std::shared_ptr<UITextField> m_velYMaxField;
	std::shared_ptr<UITextField> m_accelXField;
	std::shared_ptr<UITextField> m_accelYField;
	std::shared_ptr<UITextField> m_sizeField;
	std::shared_ptr<UITextField> m_durationMinField;
	std::shared_ptr<UITextField> m_durationMaxField;

	// Visuals panel elements.
	std::shared_ptr<UIContainer> m_visualsPanel;
	std::shared_ptr<UISlider> m_rSlider;
	std::shared_ptr<UISlider> m_gSlider;
	std::shared_ptr<UISlider> m_bSlider;
	std::shared_ptr<UISlider> m_birthRSlider;
	std::shared_ptr<UISlider> m_birthGSlider;
	std::shared_ptr<UISlider> m_birthBSlider;
	std::shared_ptr<UISlider> m_deathRSlider;
	std::shared_ptr<UISlider> m_deathGSlider;
	std::shared_ptr<UISlider> m_deathBSlider;

	// Movement panel elements.
	std::shared_ptr<UIContainer> m_movementPanel;
	std::shared_ptr<UITextField> m_particleSpeedField;
	std::shared_ptr<UITextField> m_hSinAmountField;
	std::shared_ptr<UITextField> m_hSinPeriodField;
	std::shared_ptr<UITextField> m_vSinAmountField;
	std::shared_ptr<UITextField> m_vSinPeriodField;
	std::shared_ptr<UITextField> m_circleAmountField;
	std::shared_ptr<UITextField> m_circlePeriodField;

	// Emitters panel elements.
	std::shared_ptr<UIContainer> m_emittersPanel;

	// Render panel elements.
	std::shared_ptr<UIContainer> m_renderPanel;
	std::shared_ptr<UITextField> m_clipXField;
	std::shared_ptr<UITextField> m_clipYField;
	std::shared_ptr<UITextField> m_durationField;
	std::shared_ptr<UITextField> m_fpsField;
};
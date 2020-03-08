#pragma once

#include "IState.h"
#include "UIRenderer.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Engine;
class EmitterRenderer;
class TextRenderer;
class IUserInterface;
class UIContainer;
class UISlider;
class UITextField;

class EditorState : public IState
{
public:
	EditorState(Engine *engine, std::shared_ptr<EmitterRenderer> eRenderer, 
		TextRenderer *tRenderer, UIRenderer *uRenderer);

	virtual void handleInput(InputManager *inputManager);

	virtual void update(Engine *engine, float deltaTime);

	void updateUIFromEmitter(Engine *emitter, int index);

private:
	// Initialize UI elements.
	void initTopLeftPanel(Engine *engine, TextRenderer *tRenderer, 
		UIRenderer *uRenderer);
	void initTopRightPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);
	void initBottomPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);
	void initParticlesPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);
	void initVisualsPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);
	void initMovementPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);
	void initEmittersPanel(Engine *engine, TextRenderer *tRenderer, 
		UIRenderer *uRenderer);
	void initRenderPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);

	// Calculate the size of the viewport.
	glm::ivec2 getViewportSize(glm::ivec2 windowSize) const;

	// Update the position of the clip box.
	void updateClipBoxPos(glm::ivec2 viewportSize);

	// Store the current window size.
	// Use this to detect window size changes and then rescale UI.
	glm::vec2 m_windowSize{ 0.f };

	// The index of the currently selected emitter.
	int m_emitter{ 0 };

	// Pointer to the emitter renderer.
	std::shared_ptr<EmitterRenderer> m_eRenderer{ nullptr };

	// The clip size's visual representation.
	UIRenderer::Properties *m_clipSizeBox{ nullptr };

	// UI Elements for editing emitters.
	std::vector<std::shared_ptr<IUserInterface>> m_uiElements;
	std::shared_ptr<UIContainer> m_topLeftPanel{ nullptr };
	std::shared_ptr<UIContainer> m_topRightPanel{ nullptr };
	std::shared_ptr<UIContainer> m_bottomPanel{ nullptr };

	// Particles panel elements.
	std::shared_ptr<UIContainer> m_particlesPanel{ nullptr };
	std::shared_ptr<UITextField> m_xField{ nullptr };
	std::shared_ptr<UITextField> m_yField{ nullptr };
	std::shared_ptr<UITextField> m_numGenField{ nullptr };
	std::shared_ptr<UITextField> m_spawnTimeField{ nullptr };
	std::shared_ptr<UITextField> m_delayField{ nullptr };
	std::shared_ptr<UITextField> m_velXMinField{ nullptr };
	std::shared_ptr<UITextField> m_velXMaxField{ nullptr };
	std::shared_ptr<UITextField> m_velYMinField{ nullptr };
	std::shared_ptr<UITextField> m_velYMaxField{ nullptr };
	std::shared_ptr<UITextField> m_accelXField{ nullptr };
	std::shared_ptr<UITextField> m_accelYField{ nullptr };
	std::shared_ptr<UITextField> m_sizeField{ nullptr };
	std::shared_ptr<UITextField> m_lifeMinField{ nullptr };
	std::shared_ptr<UITextField> m_lifeMaxField{ nullptr };

	// Visuals panel elements.
	std::shared_ptr<UIContainer> m_visualsPanel{ nullptr };
	std::shared_ptr<UISlider> m_rSlider{ nullptr };
	std::shared_ptr<UISlider> m_gSlider{ nullptr };
	std::shared_ptr<UISlider> m_bSlider{ nullptr };
	std::shared_ptr<UISlider> m_aSlider{ nullptr };
	std::shared_ptr<UISlider> m_birthRSlider{ nullptr };
	std::shared_ptr<UISlider> m_birthGSlider{ nullptr };
	std::shared_ptr<UISlider> m_birthBSlider{ nullptr };
	std::shared_ptr<UISlider> m_birthASlider{ nullptr };
	std::shared_ptr<UISlider> m_deathRSlider{ nullptr };
	std::shared_ptr<UISlider> m_deathGSlider{ nullptr };
	std::shared_ptr<UISlider> m_deathBSlider{ nullptr };
	std::shared_ptr<UISlider> m_deathASlider{ nullptr };

	// Movement panel elements.
	std::shared_ptr<UIContainer> m_movementPanel{ nullptr };
	std::shared_ptr<UITextField> m_particleSpeedField{ nullptr };
	std::shared_ptr<UITextField> m_hSinAmountField{ nullptr };
	std::shared_ptr<UITextField> m_hSinPeriodField{ nullptr };
	std::shared_ptr<UITextField> m_vSinAmountField{ nullptr };
	std::shared_ptr<UITextField> m_vSinPeriodField{ nullptr };
	std::shared_ptr<UITextField> m_circleAmountField{ nullptr };
	std::shared_ptr<UITextField> m_circlePeriodField{ nullptr };

	// Emitters panel elements.
	std::shared_ptr<UIContainer> m_emittersPanel{ nullptr };

	// Render panel elements.
	std::shared_ptr<UIContainer> m_renderPanel{ nullptr };
	std::shared_ptr<UITextField> m_clipXField{ nullptr };
	std::shared_ptr<UITextField> m_clipYField{ nullptr };
	std::shared_ptr<UITextField> m_durationField{ nullptr };
	std::shared_ptr<UITextField> m_fpsField{ nullptr };
};
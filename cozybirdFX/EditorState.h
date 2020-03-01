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
	std::shared_ptr<UIContainer> m_topPanel;
	std::shared_ptr<UIContainer> m_bottomPanel;
	std::shared_ptr<UIContainer> m_sidePanel;
	std::shared_ptr<UISlider> m_rSlider;
	std::shared_ptr<UISlider> m_gSlider;
	std::shared_ptr<UISlider> m_bSlider;
	std::shared_ptr<UITextField> m_xField;
	std::shared_ptr<UITextField> m_yField;
	std::shared_ptr<UITextField> m_numGenField;
	std::shared_ptr<UITextField> m_spawnTimeField;
	std::shared_ptr<UITextField> m_velXMinField;
	std::shared_ptr<UITextField> m_velXMaxField;
	std::shared_ptr<UITextField> m_velYMinField;
	std::shared_ptr<UITextField> m_velYMaxField;
	std::shared_ptr<UITextField> m_accelXField;
	std::shared_ptr<UITextField> m_accelYField;
	std::shared_ptr<UITextField> m_sizeField;
	std::shared_ptr<UITextField> m_durationMinField;
	std::shared_ptr<UITextField> m_durationMaxField;
};
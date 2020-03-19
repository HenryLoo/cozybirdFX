#pragma once

#include <glm/glm.hpp>

class Camera;
class Engine;
class InputManager;

class IState
{
public:
	// Handle inputs for this state.
	virtual void handleInput(InputManager &inputManager) = 0;

	// Update common values for this state.
	void update(float deltaTime, Engine &engine);

	// Render this state.
	virtual void render(float deltaTime, const Camera &camera) = 0;

protected:
	// Store the current window size.
	// Use this to detect window size changes and then rescale UI.
	glm::ivec2 m_windowSize{ 0, 0 };

private:
	// Update specific subclass values for this state.
	virtual void update(float deltaTime) = 0;

	// Handle changes when resizing the window.
	virtual void resize(Camera &camera) = 0;
};
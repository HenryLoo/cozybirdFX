#pragma once

class Camera;
class Engine;
class InputManager;

class IState
{
public:
	// Handle inputs for this state.
	virtual void handleInput(InputManager &inputManager) = 0;

	// Update values for this state.
	virtual void update(float deltaTime, Engine &engine) = 0;

	// Render this state.
	virtual void render(float deltaTime, const Camera &camera) = 0;
};
#pragma once

class Engine;
class InputManager;

class IState
{
public:
	// Handle inputs for this state.
	virtual void handleInput(InputManager *inputManager) = 0;

	// Update values for this state.
	virtual void update(Engine *engine, float deltaTime) = 0;
};
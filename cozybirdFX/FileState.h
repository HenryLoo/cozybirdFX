#pragma once

#include "IState.h"

class Camera;
class Engine;
class InputManager;

class FileState : public IState
{
public:
	virtual void handleInput(InputManager &inputManager) override;

	virtual void update(float deltaTime, Engine &engine) override;

	virtual void render(float deltaTime, const Camera &camera) override;
};
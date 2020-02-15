#pragma once

class InputManager;
class UIRenderer;

class IUserInterface
{
public:
	virtual void handleInput(InputManager *inputManager) = 0;
	virtual void addToRenderer(UIRenderer *uRenderer) = 0;
};
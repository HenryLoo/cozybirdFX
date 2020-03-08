#pragma once

class Camera;

class IRenderer
{
public:
	// Render all elements.
	virtual void render(float deltaTime, Camera *camera) = 0;
};
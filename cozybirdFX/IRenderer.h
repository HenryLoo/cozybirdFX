#pragma once

class Camera;

class IRenderer
{
public:
	// Update dynamic attributes of all elements.
	virtual void update(float deltaTime) = 0;

	// Render all elements.
	virtual void render(Camera *camera) = 0;
};
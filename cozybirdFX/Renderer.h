#pragma once

class Renderer
{
public:
	// Update dynamic attributes of all elements.
	virtual void update() = 0;

	// Render all elements.
	virtual void render() = 0;

private:

};
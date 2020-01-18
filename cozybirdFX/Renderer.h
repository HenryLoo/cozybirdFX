#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

class Renderer
{
public:
	// Update dynamic attributes of all elements.
	virtual void update(float deltaTime) = 0;

	// Render all elements.
	virtual void render() = 0;

	static void updateWindowSize(GLFWwindow *window);

protected:
	static glm::ivec2 m_windowSize;
};
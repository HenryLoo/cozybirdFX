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
	// Get projection matrices.
	glm::mat4 getPerspectiveMatrix() const;
	glm::mat4 getOrthographicMatrix() const;

	static glm::ivec2 m_windowSize;
};
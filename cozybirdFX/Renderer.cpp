#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

glm::ivec2 Renderer::m_windowSize;

void Renderer::updateWindowSize(GLFWwindow *window)
{
	glfwGetWindowSize(window, &m_windowSize.x, &m_windowSize.y);
}

glm::mat4 Renderer::getPerspectiveMatrix() const
{
	glm::mat4 proj{ 1.f };

	// Avoid divide by 0.
	if (m_windowSize.y > 0)
	{
		proj = glm::perspective(glm::radians(45.0f),
			(float)m_windowSize.x / m_windowSize.y, 0.1f, 100.0f);
	}

	return proj;
}

glm::mat4 Renderer::getOrthographicMatrix() const
{
	return glm::ortho(
		0.0f,
		static_cast<float>(m_windowSize.x),
		static_cast<float>(m_windowSize.y),
		0.0f);
}

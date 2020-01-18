#include "Renderer.h"

#include <GLFW/glfw3.h>

glm::ivec2 Renderer::m_windowSize;

void Renderer::updateWindowSize(GLFWwindow *window)
{
	glfwGetWindowSize(window, &m_windowSize.x, &m_windowSize.y);
}
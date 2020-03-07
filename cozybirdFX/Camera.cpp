#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec2 size, float zoom) :
	m_size(size), m_zoom(zoom)
{
	updateView();
	updateProjection();
}

void Camera::setPosition(glm::vec2 position)
{
	m_position = position;

	// Update the view matrix after changing the position.
	updateView();
}

void Camera::setSize(glm::vec2 size)
{
	m_size = size;

	// Update projection matrices after changing the size.
	updateProjection();
}

void Camera::setZoom(float zoom)
{
	m_zoom = zoom;
}

glm::mat4 Camera::getView() const
{
	return m_view;
}

float Camera::getZoom() const
{
	return m_zoom;
}

glm::mat4 Camera::getUIProjection() const
{
	return m_uiProj;
}

glm::mat4 Camera::getSceneProjection() const
{
	return m_sceneProj;
}

void Camera::updateView()
{
	glm::mat4 view{ glm::mat4(1.0f) };
	view = glm::translate(view, 
		glm::vec3(m_position.x, m_position.y,  0.f));

	m_view = view;
}

void Camera::updateProjection()
{
	m_uiProj = glm::ortho(0.0f, m_size.x, m_size.y, 0.0f);
	
	glm::vec2 halfScreenSize{ m_size / (2.f * m_zoom) };
	m_sceneProj = glm::ortho(
		-halfScreenSize.x, halfScreenSize.x,
		-halfScreenSize.y, halfScreenSize.y,
		-1000.f, 1000.f);
}

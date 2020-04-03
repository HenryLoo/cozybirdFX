#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(glm::vec2 size, float zoom);

	// Setter functions.
	void setPosition(glm::vec2 position);
	void setSize(glm::vec2 size);
	void setZoom(float zoom);

	// Getter functions.
	glm::vec2 getPosition() const;
	glm::mat4 getView() const;
	float getZoom() const;

	// Get projection matrices.
	glm::mat4 getUIProjection() const;
	glm::mat4 getSceneProjection() const;

private:
	// Update the canera's matrices.
	void updateView();
	void updateProjection();

	// The center position of the camera in pixels.
	glm::vec2 m_position{ 0.f };

	// The width and height of the camera.
	glm::vec2 m_size{ 0.f };

	// The scale value for how zoomed in the camera is.
	float m_zoom{ 1.f };

	// Hold the view matrix to avoid recalculating.
	glm::mat4 m_view{ 1.f };

	// Hold the projection matrices to avoid recalculating.
	glm::mat4 m_uiProj{ 1.f };
	glm::mat4 m_sceneProj{ 1.f };
};
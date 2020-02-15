#pragma once

#include "IUserInterface.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class InputManager;
class UIRenderer;

class UIContainer : public IUserInterface
{
public:
	UIContainer(glm::vec2 position, glm::vec2 size, 
		glm::vec4 colour = { 0.3f, 0.3f, 0.5f, 0.8f }, bool hasBorder = true);

	virtual void handleInput(InputManager *inputManager);

	// Add a UI element to this container.
	void addElement(std::shared_ptr<IUserInterface> element);

	virtual void addToRenderer(UIRenderer *uRenderer);

	// Getter functions.
	glm::vec2 getPosition() const;
	glm::vec2 getSize() const;
	glm::vec4 getColour() const;
	bool hasBorder() const;

private:
	// The x, y position of this container.
	glm::vec2 m_position;

	// The width and height of this container.
	glm::vec2 m_size;

	// The colour of this container.
	glm::vec4 m_colour;

	// Flag for if this container has a border.
	bool m_hasBorder;

	// Hold other UI elements in this container.
	std::vector<std::shared_ptr<IUserInterface>> m_elements;
};
#pragma once

#include <glm/glm.hpp>

class InputManager;
class TextRenderer;
class UIRenderer;

class IUserInterface
{
public:
	IUserInterface(glm::vec2 position, glm::vec2 size, glm::vec4 colour, 
		bool hasBorder);

	virtual void handleInput(InputManager *inputManager) = 0;
	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);

	void setOffset(glm::vec2 offset);

	// Getter functions.
	glm::vec2 getPosition() const;
	glm::vec2 getSize() const;
	glm::vec4 getColour() const;
	bool hasBorder() const;

protected:
	// The x, y position of this element.
	glm::vec2 m_position{ 0.f };

	// The x, y offset. Add this to position for the final position values.
	glm::vec2 m_offset{ 0.f };

	// The width and height of this element.
	glm::vec2 m_size{ 0.f };

	// The colour of this element.
	glm::vec4 m_colour{ 1.f };

	// Flag for if this element has a border.
	bool m_hasBorder{ false };
};
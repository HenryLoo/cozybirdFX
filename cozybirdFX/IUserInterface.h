#pragma once

#include <glm/glm.hpp>
#include "UIRenderer.h"

class InputManager;
class TextRenderer;

class IUserInterface
{
public:
	IUserInterface(glm::vec2 position, glm::vec2 size, glm::vec4 colour, 
		bool hasBorder);

	void process(InputManager *inputManager);
	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);

	// Setter functions.
	virtual void setPosition(glm::vec2 position);
	virtual void setSize(glm::vec2 size);
	virtual void setOffset(glm::vec2 offset);
	virtual void setEnabled(bool isEnabled);

	// Getter functions.
	glm::vec2 getPosition() const;
	glm::vec2 getSize() const;
	glm::vec4 getColour() const;
	bool hasBorder() const;

protected:
	virtual void handleInput(InputManager *inputManager) = 0;

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

	// Flag for if this element is enabled.
	// If diabled, then ignore rendering and input handling.
	bool m_isEnabled{ true };

	// Flag for if any element is clicked.
	static bool m_isClicked;

	// Pointer to this element's UI properties.
	// This is to allow for dynamically updating values.
	UIRenderer::Properties *m_uiProperties{ nullptr };
};
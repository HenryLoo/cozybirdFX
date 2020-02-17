#pragma once

#include "IUserInterface.h"

#include <vector>
#include <memory>

class InputManager;
class UIRenderer;

class UIContainer : public IUserInterface
{
public:
	// Size.x == -1 means it is fitting the width of its contents.
	// Size.y == -1 means it is fitting the height of its contents.
	UIContainer(glm::vec2 position, glm::vec2 size, 
		glm::vec4 colour = { 0.3f, 0.3f, 0.5f, 0.8f }, bool hasBorder = true);

	virtual void handleInput(InputManager *inputManager);

	// Add a UI element to this container.
	virtual void addElement(std::shared_ptr<IUserInterface> element);

	// Add a break so that the next element is added on a new line.
	void addNewLine();

	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);

private:
	void fitContents();

	// Hold other UI elements in this container.
	std::vector<std::shared_ptr<IUserInterface>> m_elements;

	// Determine the offset of the container's elements, relative to the
	// container's position.
	static const float SPACE;
	glm::vec2 m_innerOffset{ SPACE };

	// The largest total width in a row.
	float m_maxElementWidth{ 0.f };

	// The tallest element in the current row.
	float m_maxElementHeight{ 0.f };

	// Flag for if this container is fitting its contents.
	bool m_isFittingWidth{ false };
	bool m_isFittingHeight{ false };

	// Flag for if a line break has been added.
	// This is to disallow multiple breaks.
	bool m_hasLineBreak{ false };
};
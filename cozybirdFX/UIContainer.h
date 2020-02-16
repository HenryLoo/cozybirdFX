#pragma once

#include "IUserInterface.h"

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

	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);

private:
	// Hold other UI elements in this container.
	std::vector<std::shared_ptr<IUserInterface>> m_elements;
};
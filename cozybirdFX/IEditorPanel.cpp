#include "IEditorPanel.h"
#include "InputManager.h"

#include <GLFW\glfw3.h>

const glm::vec2 IEditorPanel::ONE_VAL_SIZE{ 356.f, 24.f };
const glm::vec2 IEditorPanel::TWO_VAL_SIZE{ 170.f, ONE_VAL_SIZE.y };
const glm::vec2 IEditorPanel::THREE_VAL_SIZE{ 108.f, ONE_VAL_SIZE.y };
const glm::vec2 IEditorPanel::LABEL_SIZE{ 200.f, 16.f };
const glm::vec2 IEditorPanel::BUTTON_SIZE{ 106.f, 32.f };
const glm::vec2 IEditorPanel::ONE_BUTTON_SIZE{ ONE_VAL_SIZE.x, BUTTON_SIZE.y };
const glm::vec2 IEditorPanel::TWO_BUTTON_SIZE{ TWO_VAL_SIZE.x, BUTTON_SIZE.y };
const glm::ivec2 IEditorPanel::ANGLE_RANGE{ 0, 359 };

const glm::vec2 IEditorPanel::NOT_CLICKED{ -1.f };

bool IEditorPanel::handleInput(InputManager &inputManager,
	UndoableAction &action)
{
	m_clickedPos = NOT_CLICKED;
	if (inputManager.isMouseDown(GLFW_MOUSE_BUTTON_2))
		m_clickedPos = inputManager.getMousePos();
	
	return m_panel->process(inputManager, action);
}

void IEditorPanel::setPosition(glm::vec2 position)
{
	m_panel->setPosition(position);
}

void IEditorPanel::setSize(glm::vec2 size)
{
	m_panel->setSize(size);
}

void IEditorPanel::setEnabled(bool isEnabled)
{
	m_panel->setEnabled(isEnabled);
}

glm::vec2 IEditorPanel::getPosition() const
{
	return m_panel->getPosition();
}

glm::vec2 IEditorPanel::getSize() const
{
	return m_panel->getSize();
}

void IEditorPanel::getDescription(InputManager &inputManager, 
	std::string &output) const
{
	m_panel->getDescription(inputManager, output);

	// If no element is hovered, show the panel's description instead.
	if (output.empty())
		output = m_description;
}

bool IEditorPanel::hasClicked() const
{
	return m_clickedPos != NOT_CLICKED;
}
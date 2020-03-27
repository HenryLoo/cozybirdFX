#pragma once

#include "UIContainer.h"

#include <glm/glm.hpp>

#include <memory>

class Emitter;
class InputManager;
class IUserInterface;

class IEditorPanel
{
public:
	void handleInput(InputManager &inputManager);

	virtual void update(float deltaTime, Emitter &emitter) = 0;

	virtual void updateUIFromEmitter(const Emitter &emitter) = 0;

	void setPosition(glm::vec2 position);
    void setSize(glm::vec2 size);
    void setEnabled(bool isEnabled);

    glm::vec2 getPosition() const;
    glm::vec2 getSize() const;

protected:
    // Check if the right mouse has clicked during this frame.
    bool hasClicked() const;

    // Fixed UI element sizes.
    static const glm::vec2 ONE_VAL_SIZE;
    static const glm::vec2 TWO_VAL_SIZE;
    static const glm::vec2 THREE_VAL_SIZE;
    static const glm::vec2 LABEL_SIZE;
    static const glm::vec2 BUTTON_SIZE;
    static const glm::vec2 ONE_BUTTON_SIZE;
    static const glm::vec2 TWO_BUTTON_SIZE;
    static const glm::ivec2 ANGLE_RANGE;

	std::unique_ptr<UIContainer> m_panel;

    // The screen position of the last right-click.
    static const glm::vec2 NOT_CLICKED;
    glm::vec2 m_clickedPos{ NOT_CLICKED };
};
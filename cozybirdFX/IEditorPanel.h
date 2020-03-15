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
	void handleInput(InputManager *inputManager);

	virtual void update(Emitter *emitter, float deltaTime) = 0;

	virtual void updateUIFromEmitter(Emitter *emitter) = 0;

	void setPosition(glm::vec2 position);
    void setSize(glm::vec2 size);
    void setEnabled(bool isEnabled);

    glm::vec2 getPosition() const;
    glm::vec2 getSize() const;

protected:
    // Fixed UI element sizes.
    static const glm::vec2 ONE_VAL_SIZE;
    static const glm::vec2 TWO_VAL_SIZE;
    static const glm::vec2 LABEL_SIZE;
    static const glm::vec2 COLOUR_RANGE;
    static const glm::vec2 COLOUR_SIZE;
    static const glm::vec2 BUTTON_SIZE;
    static const glm::vec2 ONE_BUTTON_SIZE;
    static const glm::vec2 TWO_BUTTON_SIZE;

	std::unique_ptr<UIContainer> m_panel;
};
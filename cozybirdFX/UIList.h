//#pragma once
//
//#include "UIContainer.h"
//
//class UIList : public UIContainer
//{
//public:
//	UIList(glm::vec2 position, glm::vec2 size);
//
//	virtual void handleInput(InputManager *inputManager);
//	virtual void addElement(std::shared_ptr<IUserInterface> element);
//	virtual void addToRenderer(UIRenderer *uRenderer, TextRenderer *tRenderer);
//
//	// Update the position of the scroll handle.
//	void updateHandle();
//
//private:
//	glm::vec2 m_innerOffset{ 16.f };
//
//	// Flag for if the left click is held down.
//	// This allows for dragging the slider even if the cursor is not inside the
//	// slider bar.
//	bool m_isClicked{ false };
//
//	int m_firstIndex{ 0 };
//	float *m_barPosY;
//};
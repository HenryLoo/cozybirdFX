#pragma once

#include "IUserInterface.h"
#include "SpriteRenderer.h"

#include <glm/glm.hpp>

class Texture;

class UISprite : public IUserInterface
{
public:
	UISprite(glm::vec2 size, glm::vec2 position = { 0.f, 0.f });

	virtual void addToRenderer(SpriteRenderer &sRenderer, 
		UIRenderer &uRenderer, TextRenderer &tRenderer) override;

	virtual void setPosition(glm::vec2 position) override;
	virtual void setEnabled(bool isEnabled) override;

	void setSprite(std::shared_ptr<Texture> texture);

protected:
	virtual bool handleInput(InputManager &inputManager,
		UndoableAction &action) override;

private:
	std::shared_ptr<Texture> m_texture{ nullptr };

	// Pointer to the sprite.
	// This allows for dynamic changes.
	SpriteRenderer::Properties *m_sProperties{ nullptr };
};
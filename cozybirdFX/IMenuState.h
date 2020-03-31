#pragma once

#include "IState.h"
#include "UIContainer.h"

#include <memory>

class AssetLoader;
class Camera;
class EmitterRenderer;
class Engine;
class InputManager;
class SpriteRenderer;
class TextRenderer;
class UIRenderer;
class UIButton;
class UIText;

class IMenuState : public IState
{
public:
	IMenuState(Engine &engine, AssetLoader &assetLoader,
		EmitterRenderer &eRenderer, const std::string &title);

	virtual void handleInput(InputManager &inputManager) override;

	virtual void render(float deltaTime, const Camera &camera) override;

	virtual void enter() override;

protected:
	// Add a UI button to the panel.
	void addButton(std::shared_ptr<UIButton> button);

	// Append the file format to the input path if it doesn't end with the
	// file format.
	static void getFilePath(std::string &path, const std::string &fileFormat);

	static const glm::vec2 BUTTON_SIZE;

	Engine &m_engine;
	AssetLoader &m_assetLoader;
	EmitterRenderer &m_eRenderer;

	// This state's renderers.
	std::shared_ptr<TextRenderer> m_tRenderer{ nullptr };
	std::shared_ptr<UIRenderer> m_uRenderer{ nullptr };
	std::shared_ptr<SpriteRenderer> m_sRenderer{ nullptr };

private:
	virtual void update(float deltaTime, const Camera &camera) override;
	virtual void resize(Camera &camera) override;

	virtual void initMenu() = 0;

	bool m_isInitialized{ false };

	// UI elements.
	std::unique_ptr<UIContainer> m_panel{ nullptr };
	std::shared_ptr<UIText> m_title{ nullptr };
	std::shared_ptr<UIButton> m_closeButton{ nullptr };
	std::vector<std::shared_ptr<UIButton>> m_buttons;
};
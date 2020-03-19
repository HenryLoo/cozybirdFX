#pragma once

#include "IState.h"
#include "UIContainer.h"

#include <memory>

class AssetLoader;
class Camera;
class Engine;
class InputManager;
class TextRenderer;
class UIRenderer;
class UIButton;
class UIText;

class FileState : public IState
{
public:
	FileState(Engine &engine, AssetLoader &assetLoader);

	virtual void handleInput(InputManager &inputManager) override;

	virtual void render(float deltaTime, const Camera &camera) override;

private:
	virtual void update(float deltaTime) override;

	virtual void resize(Camera &camera) override;

	// This state's renderers.
	std::shared_ptr<TextRenderer> m_tRenderer{ nullptr };
	std::shared_ptr<UIRenderer> m_uRenderer{ nullptr };

	// UI elements.
	std::unique_ptr<UIContainer> m_panel{ nullptr };
	std::shared_ptr<UIText> m_title{ nullptr };
	std::shared_ptr<UIButton> m_loadButton{ nullptr };
	std::shared_ptr<UIButton> m_saveButton{ nullptr };
	std::shared_ptr<UIButton> m_closeButton{ nullptr };
};
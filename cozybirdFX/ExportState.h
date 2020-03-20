#pragma once

#include "IState.h"
#include "UIContainer.h"

#include <memory>

class AssetLoader;
class Camera;
class EmitterRenderer;
class Engine;
class InputManager;
class TextRenderer;
class UIRenderer;
class UIButton;
class UIText;

class ExportState : public IState
{
public:
	ExportState(Engine &engine, AssetLoader &assetLoader,
		EmitterRenderer &eRenderer);

	virtual void handleInput(InputManager &inputManager) override;

	virtual void render(float deltaTime, const Camera &camera) override;

	virtual void enter() override;

private:
	virtual void update(float deltaTime) override;

	virtual void resize(Camera &camera) override;

	// This state's renderers.
	std::shared_ptr<TextRenderer> m_tRenderer{ nullptr };
	std::shared_ptr<UIRenderer> m_uRenderer{ nullptr };

	// UI elements.
	std::unique_ptr<UIContainer> m_panel{ nullptr };
	std::shared_ptr<UIText> m_title{ nullptr };
	std::shared_ptr<UIButton> m_sheetButton{ nullptr };
	std::shared_ptr<UIButton> m_gifButton{ nullptr };
	std::shared_ptr<UIButton> m_closeButton{ nullptr };
};
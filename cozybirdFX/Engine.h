#pragma once

#include "AssetLoader.h"
#include "EntityManager.h"
#include "Emitter.h"
#include "InputManager.h"

#include <memory>
#include <vector>

class Renderer;
class Font;
class IUserInterface;
class UISlider;
class UITextField;

struct GLFWwindow;

class Engine
{
public:
	Engine(GLFWwindow *window);
	~Engine();

	// Start the engine's loop.
	void start();

	// Flag the window size to be updated.
	void updateNewWindowSize();

private:
	// Consult the input manager to read inputs.
	void handleInput();

	// Update all dynamic values.
	void update(float deltaTime);

	// Consult all renderers to draw elements to the current scene.
	void render(float deltaTime);

	// The application window.
	GLFWwindow *m_window{ nullptr };

	// Hold all renderers.
	std::vector<std::unique_ptr<Renderer>> m_renderers;

	bool m_hasNewWindowSize{ true };

	std::unique_ptr<AssetLoader> m_assetLoader;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<InputManager> m_inputManager;

	std::unique_ptr<Emitter> m_emitter;

	// TODO: Remove this later.
	std::vector<std::shared_ptr<IUserInterface>> m_uiElements;
	std::shared_ptr<UISlider> m_rSlider;
	std::shared_ptr<UISlider> m_gSlider;
	std::shared_ptr<UISlider> m_bSlider;
	std::shared_ptr<UITextField> m_xField;
};
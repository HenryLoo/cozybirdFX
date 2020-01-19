#pragma once

#include "AssetLoader.h"

#include <memory>
#include <vector>

class Renderer;

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
	void render();

	// The application window.
	GLFWwindow *m_window{ nullptr };

	// Hold all renderers.
	std::vector<std::unique_ptr<Renderer>> m_renderers;

	bool m_hasNewWindowSize{ true };

	std::unique_ptr<AssetLoader> m_assetLoader;
};
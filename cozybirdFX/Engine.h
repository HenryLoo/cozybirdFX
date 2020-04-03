#pragma once

#include "AssetLoader.h"
#include "Camera.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "IState.h"

#include <memory>
#include <stack>
#include <vector>

class IRenderer;
class Emitter;
class EmitterRenderer;

struct GLFWwindow;

class Engine
{
public:
	Engine(GLFWwindow *window);
	~Engine();

	// Start the engine's loop.
	void start();

	// Push/pop a state to/from the states stack.
	void pushState(IState *state);
	void popState();

	// Get the current state.
	IState *getState() const;

	// Flag the window size to be updated.
	void updateNewWindowSize();

	// Get the camera.
	Camera &getCamera() const;

	// Get the window size.
	glm::ivec2 getWindowSize() const;

	// Set the window's title.
	void setWindowTitle(const std::string &title = "");

	// Set the viewport to the window size.
	void updateViewport();

private:
	// Consult the input manager to read inputs.
	void handleInput();

	// Update all dynamic values.
	void update(float deltaTime);

	// Consult all renderers to draw elements to the current scene.
	void render(float deltaTime);

	// The application window.
	GLFWwindow *m_window{ nullptr };

	// The application's states. The current state is at the top of the stack.
	// This allows for easy unwinding of states.
	std::stack<std::unique_ptr<IState>> m_states;

	std::unique_ptr<Camera> m_camera{ nullptr };

	std::unique_ptr<AssetLoader> m_assetLoader{ nullptr };
	std::unique_ptr<EntityManager> m_entityManager{ nullptr };
	std::unique_ptr<InputManager> m_inputManager{ nullptr };
};
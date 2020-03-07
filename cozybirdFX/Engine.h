#pragma once

#include "AssetLoader.h"
#include "Camera.h"
#include "EntityManager.h"
#include "Emitter.h"
#include "InputManager.h"
#include "IState.h"

#include <memory>
#include <stack>
#include <vector>

class IRenderer;
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

	// Push/pop a state to/from the states stack.
	void pushState(IState *state);
	void popState();

	// Get the current state.
	IState *getState() const;

	// Flag the window size to be updated.
	void updateNewWindowSize();

	// Get the current emitter.
	Emitter *getEmitter() const;

	// Get the camera.
	Camera *getCamera() const;

	// Get the window size.
	glm::ivec2 getWindowSize() const;

	// Prepare to export the current emitter effects as a sprite sheet.
	void exportSpriteSheet();

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
	std::vector<std::unique_ptr<IRenderer>> m_renderers;

	// Flag for if the window was resized.
	bool m_hasNewWindowSize{ true };

	// The application's states. The current state is at the top of the stack.
	// This allows for easy unwinding of states.
	std::stack<std::unique_ptr<IState>> m_states;

	std::unique_ptr<Camera> m_camera{ nullptr };

	std::unique_ptr<AssetLoader> m_assetLoader{ nullptr };
	std::unique_ptr<EntityManager> m_entityManager{ nullptr };
	std::unique_ptr<InputManager> m_inputManager{ nullptr };

	std::unique_ptr<Emitter> m_emitter{ nullptr };
};
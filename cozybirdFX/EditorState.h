#pragma once

#include "IState.h"
#include "UIRenderer.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Camera;
class Engine;
class EmitterRenderer;
class TextRenderer;

class IEditorPanel;
class TopLeftPanel;
class TopRightPanel;
class BottomPanel;
class ParticlesPanel;
class VisualsPanel;
class MovementPanel;
class EmittersPanel;
class RenderPanel;

class EditorState : public IState
{
public:
	EditorState(Engine *engine, AssetLoader *assetLoader,
		std::shared_ptr<EmitterRenderer> eRenderer, 
		TextRenderer *tRenderer, UIRenderer *uRenderer);

	virtual void handleInput(InputManager *inputManager);

	virtual void update(Engine *engine, float deltaTime);

	void selectEmitter(Engine *engine, int index);

	// Update the position of the clip box.
	void updateClipBoxPos();

private:
	void resize(glm::vec2 windowSize, Camera *camera);

	// Calculate the size of the viewport.
	glm::ivec2 getViewportSize() const;

	// Store the current window size.
	// Use this to detect window size changes and then rescale UI.
	glm::vec2 m_windowSize{ 0.f };

	// The index of the currently selected emitter.
	int m_emitter{ 0 };

	// Pointer to the emitter renderer.
	std::shared_ptr<EmitterRenderer> m_eRenderer{ nullptr };

	// The clip size's visual representation.
	UIRenderer::Properties *m_clipSizeBox{ nullptr };

	// UI Elements for editing emitters.
	std::vector<std::shared_ptr<IEditorPanel>> m_panels;
	std::shared_ptr<TopLeftPanel> m_topLeftPanel{ nullptr };
	std::shared_ptr<TopRightPanel> m_topRightPanel{ nullptr };
	std::shared_ptr<BottomPanel> m_bottomPanel{ nullptr };
	std::shared_ptr<ParticlesPanel> m_particlesPanel{ nullptr };
	std::shared_ptr<VisualsPanel> m_visualsPanel{ nullptr };
	std::shared_ptr<MovementPanel> m_movementPanel{ nullptr };
	std::shared_ptr<EmittersPanel> m_emittersPanel{ nullptr };
	std::shared_ptr<RenderPanel> m_renderPanel{ nullptr };
};
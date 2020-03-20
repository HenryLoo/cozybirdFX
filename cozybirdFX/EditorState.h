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
	EditorState(Engine &engine, AssetLoader &assetLoader);

	virtual void handleInput(InputManager &inputManager) override;

	virtual void render(float deltaTime, const Camera &camera) override;

	virtual void enter() override;

	void selectEmitter(int index);

	// Update the position of the clip box.
	void updateClipBoxPos();

private:
	virtual void update(float deltaTime) override;

	virtual void resize(Camera &camera) override;

	// Calculate the size of the viewport.
	glm::ivec2 getViewportSize() const;

	// This state's renderers.
	std::shared_ptr<TextRenderer> m_tRenderer{ nullptr };
	std::shared_ptr<UIRenderer> m_uRenderer{ nullptr };
	std::shared_ptr<EmitterRenderer> m_eRenderer{ nullptr };

	// The index of the currently selected emitter.
	int m_emitter{ 0 };

	// The clip size's visual representation.
	UIRenderer::Properties *m_clipSizeBox;

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
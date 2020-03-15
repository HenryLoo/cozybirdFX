#pragma once

#include "IEditorPanel.h"

class Engine;
class EmitterRenderer;
class TextRenderer;
class UIRenderer;

class ParticlesPanel;
class VisualsPanel;
class MovementPanel;
class EmittersPanel;
class RenderPanel;

class TopRightPanel : public IEditorPanel
{
public:
	TopRightPanel(TextRenderer *tRenderer, UIRenderer *uRenderer,
		std::shared_ptr<ParticlesPanel> particles, 
		std::shared_ptr<VisualsPanel> visuals, 
		std::shared_ptr<MovementPanel> movement, 
		std::shared_ptr<EmittersPanel> emitters, 
		std::shared_ptr<RenderPanel> render);

	virtual void update(Emitter *emitter, float deltaTime) override;

	virtual void updateUIFromEmitter(Emitter *emitter) override;
};
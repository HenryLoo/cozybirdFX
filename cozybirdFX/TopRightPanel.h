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
	TopRightPanel(TextRenderer &tRenderer, UIRenderer &uRenderer,
		ParticlesPanel &particles, VisualsPanel &visuals, 
		MovementPanel &movement, EmittersPanel &emitters, 
		RenderPanel &render);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;
};
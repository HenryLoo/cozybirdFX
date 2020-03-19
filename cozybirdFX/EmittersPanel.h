#pragma once

#include "IEditorPanel.h"

class EditorState;

class EmitterRenderer;
class TextRenderer;
class UIRenderer;

class EmittersPanel : public IEditorPanel
{
public:
	EmittersPanel(EditorState &editor, EmitterRenderer &eRenderer, 
		TextRenderer &tRenderer, UIRenderer &uRenderer);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;
};
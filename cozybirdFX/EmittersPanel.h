#pragma once

#include "IEditorPanel.h"

class EditorState;
class Engine;
class TextRenderer;
class UIRenderer;

class EmittersPanel : public IEditorPanel
{
public:
	EmittersPanel(EditorState *editor, Engine *engine, TextRenderer *tRenderer, 
		UIRenderer *uRenderer);

	virtual void update(Emitter *emitter, float deltaTime) override;

	virtual void updateUIFromEmitter(Emitter *emitter) override;
};
#pragma once

#include "IEditorPanel.h"

class TextRenderer;
class UIRenderer;

class BottomPanel : public IEditorPanel
{
public:
	BottomPanel(TextRenderer *tRenderer, UIRenderer *uRenderer);

	virtual void update(Emitter *emitter, float deltaTime) override;

	virtual void updateUIFromEmitter(Emitter *emitter) override;
};
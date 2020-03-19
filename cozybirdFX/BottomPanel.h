#pragma once

#include "IEditorPanel.h"

class TextRenderer;
class UIRenderer;

class BottomPanel : public IEditorPanel
{
public:
	BottomPanel(TextRenderer &tRenderer, UIRenderer &uRenderer);

	virtual void update(float deltaTime, Emitter &emitter) override;

	virtual void updateUIFromEmitter(const Emitter &emitter) override;
};
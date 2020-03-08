#pragma once

#include "IRenderer.h"
#include "Shader.h"
#include "Emitter.h"

#include <vector>
#include <memory>

class AssetLoader;

class EmitterRenderer : public IRenderer
{
public:
	EmitterRenderer(AssetLoader *assetLoader);

	virtual void render(float deltaTime, Camera *camera);

	// Get emitter by index.
	Emitter *getEmitter(int index) const;

	// Toggle an emitter.
	void toggleEmitter(int index, bool isEnabled);

	// Prepare to export the current emitter effects as a sprite sheet.
	void exportSpriteSheet();

	static const int NUM_EMITTERS;

private:
	// The renderer's shader program.
	std::shared_ptr<Shader> m_updateShader{ nullptr };
	std::shared_ptr<Shader> m_renderShader{ nullptr };

	// Hold emitters to render.
	std::vector<std::unique_ptr<Emitter>> m_emitters;

	// Hold flags for each emitter to indicate whether they are enabled.
	std::vector<bool> m_isEnabled;
};
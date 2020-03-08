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
	// The window size is used to reset the viewport after rendering.
	void exportSpriteSheet(glm::ivec2 windowSize);

	static const int NUM_EMITTERS;

private:
	// Create a frame buffer object for rendering to texture.
	void createFramebuffer(glm::ivec2 textureSize);

	// The renderer's shader program.
	std::shared_ptr<Shader> m_updateShader{ nullptr };
	std::shared_ptr<Shader> m_renderShader{ nullptr };

	// Hold emitters to render.
	std::vector<std::unique_ptr<Emitter>> m_emitters;

	// Hold flags for each emitter to indicate whether they are enabled.
	std::vector<bool> m_isEnabled;

	// The duration of the animation.
	float m_currentTime{ 0.f };
	float m_duration{ 3.f };

	// The size of each clip in the animation.
	glm::ivec2 m_clipSize{ 400.f, 400.f };

	// Buffers for rendering to texture.
	unsigned int m_fbo{ 0 };
	std::shared_ptr<Texture> m_fboTexture{ nullptr };
};
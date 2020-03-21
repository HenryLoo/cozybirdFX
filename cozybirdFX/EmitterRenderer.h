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
	EmitterRenderer(AssetLoader &assetLoader);

	virtual void render(float deltaTime, const Camera &camera) override;

	// Reinitialize emitters.
	void init(AssetLoader &assetLoader);

	// Getter functions.
	Emitter &getEmitter(int index) const;
	glm::ivec2 getClipSize() const;
	float getCurrentTime() const;
	float getDuration() const;
	int getExportFPS() const;
	bool isLooping() const;
	bool isPlaying() const;

	// Check if an emitter is enabled.
	bool isEnabled(int emitterIndex) const;

	// Setter functions.
	void setClipSize(glm::ivec2 size);
	void setDuration(float duration);
	void setExportFPS(int fps);
	void setLooping(bool isLooping);

	// Set whether the animation is playing or not.
	void setPlaying(bool isPlaying);

	// Toggle an emitter.
	void toggleEmitter(int index, bool isEnabled);

	// Prepare to export the current emitter effects as a sprite sheet.
	// The window size is used to reset the viewport after rendering.
	void exportSpriteSheet(glm::ivec2 windowSize, const std::string &outputPath);

	// Prepare to export the current emitter effects as an animated gif.
	// The window size is used to reset the viewport after rendering.
	void exportGif(glm::ivec2 windowSize, const std::string &outputPath);

	static const int NUM_EMITTERS;

private:
	// Create a frame buffer object for rendering to texture.
	void createFramebuffer(glm::ivec2 textureSize);
	void createFramebufferTexture(glm::ivec2 textureSize);

	// Reset the animation.
	void reset();

	// Prepare common export attributes.
	void prepareExport(glm::ivec2 textureSize);

	// Calculate the deltaTime to use when exporting.
	float getFixedDeltaTime() const;

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
	glm::ivec2 m_clipSize{ 200.f, 200.f };

	// The frame rate to render the animation at when exporting.
	int m_exportFPS{ 60 };

	// Flag for if the animation is looping.
	bool m_isLooping{ true };

	// Flag for if the animation is playing.
	bool m_isPlaying{ true };

	// Buffers for rendering to texture.
	unsigned int m_fbo{ 0 };
	std::shared_ptr<Texture> m_fboTexture{ nullptr };
};
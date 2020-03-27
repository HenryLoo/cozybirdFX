#pragma once

#include "IRenderer.h"
#include "Emitter.h"

#include <vector>
#include <memory>

class AssetLoader;
class Engine;
class Shader;

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
	void setExportFPS(int fps);
	void setLooping(bool isLooping);

	// Set whether the animation is playing or not.
	void setPlaying(bool isPlaying);

	// Toggle an emitter.
	void toggleEmitter(int index, bool isEnabled);

	// Prepare to export the current emitter effects as a sprite sheet.
	// The window size is used to reset the viewport after rendering.
	void exportSpriteSheet(Engine &engine, const std::string &outputPath);

	// Prepare to export the current emitter effects as an animated gif.
	// The window size is used to reset the viewport after rendering.
	void exportGif(Engine &engine, const std::string &outputPath);

	static const int NUM_EMITTERS;

private:
	// Create a frame buffer object for rendering to texture.
	void createFramebuffers(glm::ivec2 textureSize, bool hasLayers);
	void createFramebuffer(glm::ivec2 textureSize, bool isClearBuffers);
	void createFramebufferTexture(glm::ivec2 textureSize);
	void clearFramebuffers();

	// Reset the animation.
	void reset();

	// Prepare common export attributes.
	void prepareExport(glm::ivec2 textureSize, bool hasLayers);

	// Common emitter render code.
	void render(float deltaTime, bool isOnlyUpdate = false, 
		bool hasLayers = false);

	// Calculate the deltaTime to use when exporting.
	float getFixedDeltaTime() const;

	// Update the window title based on current export progress.
	void updateExportProgress(Engine &engine, int currentVal, int total = 1) const;

	// The renderer's shader program.
	std::shared_ptr<Shader> m_updateShader{ nullptr };
	std::shared_ptr<Shader> m_renderShader{ nullptr };

	// Hold emitters to render.
	std::vector<std::unique_ptr<Emitter>> m_emitters;

	// Hold flags for each emitter to indicate whether they are enabled.
	std::vector<bool> m_isEnabled;

	// The duration of the animation.
	float m_currentTime{ 0.f };

	// The size of each clip in the animation.
	glm::ivec2 m_clipSize{ 0 };

	// The frame rate to render the animation at when exporting.
	int m_exportFPS{ 0 };

	// Flag for if the animation is looping.
	bool m_isLooping{ false };

	// Flag for if the animation is playing.
	bool m_isPlaying{ false };

	// Buffers for rendering to texture.
	std::vector<unsigned int> m_fbo;
	std::vector<std::shared_ptr<Texture>> m_fboTexture;
};
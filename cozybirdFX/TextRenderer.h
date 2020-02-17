#pragma once

#include "Renderer.h"

#include <memory>
#include <unordered_map>

class AssetLoader;
class Font;
class Shader;

class TextRenderer : public Renderer
{
public:
	enum class TextAlign
	{
		LEFT,
		CENTER,
		RIGHT
	};

	struct Properties
	{
		std::string text{ "" };

		// Top-left position.
		glm::vec2 pos{ 0.f };

		// Width and height.
		glm::vec2 size{ 0.f };

		glm::vec4 colour{ 1.f };
		TextAlign align{ TextAlign::LEFT };
		bool isVerticalCenter{ false };
		float scale{ 1.f };
	};

	TextRenderer(AssetLoader *assetLoader);
	~TextRenderer();

	virtual void update(float deltaTime);

	virtual void render();

	// Add a text to the map to be rendered.
	std::vector<Properties>::iterator addText(const Properties &prop, Font *font = nullptr);

	// Remove all text from the map.
	void clearText();

private:
	// The renderer's shader program.
	std::shared_ptr<Shader> m_shader;

	// The renderer's default font.
	std::shared_ptr<Font> m_font;

	// Vertex array object.
	unsigned int m_VAO;

	// Vertex buffer objects.
	unsigned int m_verticesVBO, m_clipVBO, m_colourVBO, m_modelVBO;

	// Element buffer object.
	unsigned int m_verticesEBO;

	// Text to render.
	// Group text strings to render by common fonts, to minimize texture binds
	// and draw calls.
	std::unordered_map<Font *, std::vector<Properties>> m_texts;
};
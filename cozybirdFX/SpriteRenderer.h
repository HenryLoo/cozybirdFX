#pragma once

#include "IRenderer.h"
#include "Shader.h"
#include "Texture.h"

#include <list>
#include <memory>

class AssetLoader;

class SpriteRenderer : public IRenderer
{
public:
	struct Properties
	{
		// The sprite texture.
		Texture *texture{ nullptr };

		// Top-left position.
		glm::vec2 pos{ 0.f };

		// Width and height.
		glm::vec2 size{ 0.f };

		// Flag for if this element should be rendered.
		bool isEnabled{ true };
	};

	SpriteRenderer(AssetLoader &assetLoader);

	virtual void render(float deltaTime, const Camera &camera) override;

	typedef std::list<Properties>::iterator PropertiesIterator;
	PropertiesIterator addSprite(const Properties &properties);

	void clearSprites();

private:
	// The renderer's shader program.
	std::shared_ptr<Shader> m_shader;

	// Vertex array object.
	unsigned int m_VAO;
	
	// Vertex buffer object.
	unsigned int m_VBO;

	// Element buffer object.
	unsigned int m_EBO;

	// Sprites to render.
	std::list<Properties> m_sprites;
};
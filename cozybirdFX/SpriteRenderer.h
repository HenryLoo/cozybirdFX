#pragma once

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

class AssetLoader;

class SpriteRenderer : public Renderer
{
public:
	SpriteRenderer(AssetLoader *assetLoader);

	virtual void update(float deltaTime);

	virtual void render();

private:
	// The renderer's shader program.
	std::shared_ptr<Shader> m_shader;

	// Vertex array object.
	unsigned int m_vao;
	
	// Vertex buffer object.
	unsigned int m_vbo;

	// Element buffer object.
	unsigned int m_ebo;

	// TODO: Remove this later.
	std::shared_ptr<Texture> m_texture;
};
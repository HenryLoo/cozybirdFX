#pragma once

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include <list>
#include <memory>

class AssetLoader;

class SpriteRenderer : public Renderer
{
public:
	SpriteRenderer(AssetLoader *assetLoader);

	virtual void update(float deltaTime);

	virtual void render();

	void addSprite(glm::mat4 model);

private:
	// The renderer's shader program.
	std::shared_ptr<Shader> m_shader;

	// Vertex array object.
	unsigned int m_VAO;
	
	// Vertex buffer object.
	unsigned int m_VBO;

	// Element buffer object.
	unsigned int m_EBO;

	// TODO: Remove this later.
	std::shared_ptr<Texture> m_texture;

	// Hold model matrices for all sprites to render.
	std::list<glm::mat4> m_models;
};
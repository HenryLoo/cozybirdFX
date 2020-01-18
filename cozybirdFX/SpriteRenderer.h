#pragma once

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include <glad/glad.h>
#include <memory>

class SpriteRenderer : public Renderer
{
public:
	SpriteRenderer();

	virtual void update(float deltaTime);

	virtual void render();

private:
	// The renderer's shader program.
	std::unique_ptr<Shader> m_shader;

	// Vertex array object.
	GLuint m_vao;
	
	// Vertex buffer object.
	GLuint m_vbo;

	// Element buffer object.
	GLuint m_ebo;

	// TODO: Remove this later.
	std::unique_ptr<Texture> m_texture;
};
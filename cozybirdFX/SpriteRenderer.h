#pragma once

#include "Renderer.h"
#include "Shader.h"

#include <glad/glad.h>
#include <memory>

class SpriteRenderer : public Renderer
{
public:
	SpriteRenderer();

	virtual void update();

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
};
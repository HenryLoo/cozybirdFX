#pragma once

#include "Renderer.h"

#include <glad/glad.h>

class SpriteRenderer : public Renderer
{
public:
	SpriteRenderer();

	virtual void update();

	virtual void render();

private:
	// The renderer's shader program.
	GLuint m_shaderProgram;

	// Vertex array object.
	GLuint m_vao;
	
	// Vertex buffer object.
	GLuint m_vbo;

	// Element buffer object.
	GLuint m_ebo;
};
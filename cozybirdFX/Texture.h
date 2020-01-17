#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
	Texture(const std::string &filePath);
	~Texture();

	// Bind to this texture.
	void bind() const;

private:
	// This texture's id.
	GLuint m_textureId{ 0 };

	// Texture properties.
	int m_width{ 0 };
	int m_height{ 0 };
	int m_numChannels{ 0 };
};
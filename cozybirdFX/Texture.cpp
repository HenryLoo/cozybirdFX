#include "Texture.h"

#include <glad/glad.h>

#include <iostream>

namespace
{
    const std::string ASSET_PATH{ "assets/" };
    const std::string TEXTURE_PATH{ "texture/" };
}

Texture::Texture(unsigned int textureId, int width, int height, int numChannels) :
    m_textureId(textureId), m_width(width), m_height(height), 
    m_numChannels(numChannels)
{

}

Texture::~Texture()
{
    glDeleteTextures(1, &m_textureId);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}
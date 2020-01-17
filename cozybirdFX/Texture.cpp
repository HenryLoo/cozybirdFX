#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

namespace
{
    const std::string ASSET_PATH{ "assets/" };
    const std::string TEXTURE_PATH{ "texture/" };
}

Texture::Texture(const std::string &filePath)
{
    // Create the texture.
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Set filtering options.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load the texture from file.
    stbi_set_flip_vertically_on_load(true);
    const std::string path{ ASSET_PATH + TEXTURE_PATH + filePath };
    unsigned char *data{ stbi_load(path.c_str(), &m_width, &m_height, &m_numChannels, 0) };
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Texture: Failed to load texture: " << filePath << std::endl;
    }

    // Clean up.
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_textureId);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

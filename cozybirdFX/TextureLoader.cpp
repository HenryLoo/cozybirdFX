#include "TextureLoader.h"
#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

namespace
{
    const std::string TEXTURE_PATH{ "texture/" };
}

TextureLoader::TextureLoader() : ITypeLoader(TEXTURE_PATH)
{

}

std::shared_ptr<IAsset> TextureLoader::interpretAsset(
    const std::vector<AssetBuffer> &data) const
{
    // Create the texture.
    GLuint textureId{ 0 };
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set filtering options.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load the texture from the buffer.
    stbi_set_flip_vertically_on_load(true);
    int width, height, numChannels;
    stbi_uc *loadedImage{ stbi_load_from_memory((stbi_uc *)data[0].buffer, 
        data[0].length, &width, &height, &numChannels, 0) };

    std::shared_ptr<Texture> texture{ nullptr };
    if (loadedImage)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImage);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Clean up.
        stbi_image_free(loadedImage);

        texture = std::make_shared<Texture>(textureId, width, height, numChannels);
    }

    // Unbind the texture.
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

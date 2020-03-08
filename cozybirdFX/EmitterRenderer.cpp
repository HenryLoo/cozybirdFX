#include "EmitterRenderer.h"
#include "AssetLoader.h"
#include "Texture.h"

#include <glad/glad.h>

#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace
{
    const int NUM_PARTICLE_ATTRIBUTES{ 6 };

    const char *PARTICLE_ATTRIBUTES[NUM_PARTICLE_ATTRIBUTES]
    {
       "gsPosition",
       "gsVelocity",
       "gsColour",
       "gsDuration",
       "gsSize",
       "gsType",
    };
}

const int EmitterRenderer::NUM_EMITTERS{ 9 };

EmitterRenderer::EmitterRenderer(AssetLoader *assetLoader)
{    
    // Create update shader program.
    m_updateShader = assetLoader->load<Shader>({ "emitter_update.vs", "", "emitter_update.gs" });
    glTransformFeedbackVaryings(m_updateShader->getProgramId(),
        NUM_PARTICLE_ATTRIBUTES, PARTICLE_ATTRIBUTES, GL_INTERLEAVED_ATTRIBS);
    m_updateShader->link();

    // Create render shader program.
    m_renderShader = assetLoader->load<Shader>({ "emitter_render.vs", "emitter_render.fs", "emitter_render.gs" });
    m_renderShader->link();

    // Calculate axes for billboarding.
    // View vector doesn't change, assuming that camera is fixed.
    m_renderShader->use();
    glm::vec3 viewVec{ 0.f, 0.f, -1.f };
    glm::vec3 upVec{ 0.f, 1.f, 0.f };
    glm::vec3 axis1{ glm::cross(viewVec, upVec) };
    axis1 = glm::normalize(axis1);
    glm::vec3 axis2{ glm::cross(viewVec, axis1) };
    axis2 = glm::normalize(axis2);
    m_renderShader->setVec3("axis1", axis1);
    m_renderShader->setVec3("axis2", axis2);

    // Initialize emitters and start with one emitter enabled.
    for (int i = 0; i < NUM_EMITTERS; ++i)
    {
        auto emitter{ std::make_unique<Emitter>() };
        emitter->setTexture(assetLoader->load<Texture>("particle.png"));
        m_emitters.push_back(std::move(emitter));
    }
    m_isEnabled.resize(m_emitters.size());
    m_isEnabled[0] = true;
}

void EmitterRenderer::render(float deltaTime, Camera *camera)
{    
    for (int i = 0; i < m_emitters.size(); ++i)
    {
        // Skip if disabled.
        if (!m_isEnabled[i])
            continue;

        auto &emitter{ m_emitters[i] };
        emitter->update(deltaTime, m_updateShader);
        emitter->render(camera, m_renderShader);
    }
}

Emitter *EmitterRenderer::getEmitter(int index) const
{
    if (m_emitters.empty())
        return nullptr;

    index = glm::clamp(index, 0, static_cast<int>(m_emitters.size()) - 1);
    return m_emitters[index].get();
}

void EmitterRenderer::toggleEmitter(int index, bool isEnabled)
{
    if (m_emitters.empty())
        return;

    index = glm::clamp(index, 0, static_cast<int>(m_emitters.size()) - 1);
    m_isEnabled[index] = isEnabled;
}

void EmitterRenderer::exportSpriteSheet()
{
    //for (int i = 0; i < m_emitters.size(); ++i)
    //{
    //    // Skip if disabled.
    //    if (!m_isEnabled[i])
    //        continue;

    //    // Draw to framebuffer's texture.
    //    auto &emitter{ m_emitters[i] };
    //    emitter->outputToTexture(m_updateShader, m_renderShader);

    //    // Write image from the texture.
    //    Texture *texture{ emitter->getOutputTexture() };
    //    if (texture != nullptr)
    //    {
    //        texture->bind();
    //        glm::ivec2 size{ texture->getWidth(), texture->getHeight() };
    //        int numChannels{ texture->getNumChannels() };

    //        stbi_uc *data{ new stbi_uc[size.x * size.y * numChannels] };
    //        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //        stbi_flip_vertically_on_write(true);
    //        std::string output{ "output.png" };
    //        stbi_write_png(output.c_str(), size.x, size.y, numChannels, data, 0);

    //        delete[] data;
    //    }
    //}
}
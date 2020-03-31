#include "SpriteRenderer.h"
#include "AssetLoader.h"
#include "Camera.h"

#include <glad/glad.h>

SpriteRenderer::SpriteRenderer(AssetLoader &assetLoader)
{
    // TODO: Clean up this initialization code.
    m_shader = assetLoader.load<Shader>({ "sprite.vs", "sprite.fs" });
    if (m_shader != nullptr)
        m_shader->link();

    // Create the vertex array object and bind to it.
    // All subsequent VBO settings will be saved to this VAO.
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the vertex buffer object and bind to it.
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Copy the vertices into the buffer for OpenGL.
    const float vertices[]
    {
        // position        // texCoord
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // top left 
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create the element buffer object and bind to it.
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    // Copy indices into the element buffer for OpenGL.
    unsigned int indices[]
    {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void SpriteRenderer::render(float deltaTime, const Camera &camera)
{
    // Enable blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // TODO: Replace this test rendering code.
    m_shader->use();
    glBindVertexArray(m_VAO);

    glm::mat4 view{ 1.f };
    glm::mat4 proj{ camera.getUIProjection() };

    for (auto it = m_sprites.begin(); it != m_sprites.end(); ++it)
    {
        if (it->texture == nullptr || !it->isEnabled)
            continue;

        it->texture->bind();
        glm::mat4 model{ glm::mat4(1.0f) };
        model = glm::translate(model, glm::vec3(it->pos.x, it->pos.y, 0.f));
        model = glm::scale(model, glm::vec3(it->size.x, it->size.y, 1.f));
        glm::mat4 mvp{ proj * view * model };
        m_shader->setMat4("mvp", mvp);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

SpriteRenderer::PropertiesIterator SpriteRenderer::addSprite(const SpriteRenderer::Properties &properties)
{
    m_sprites.push_back(properties);
    return std::prev(std::end(m_sprites));
}

void SpriteRenderer::clearSprites()
{
    m_sprites.clear();
}

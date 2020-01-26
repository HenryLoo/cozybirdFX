#include "SpriteRenderer.h"
#include "AssetLoader.h"

#include <glad/glad.h>

SpriteRenderer::SpriteRenderer(AssetLoader *assetLoader)
{
    // TODO: Clean up this initialization code.
    m_shader = assetLoader->load<Shader>({ "sprite.vs", "sprite.fs" });
    m_texture = assetLoader->load<Texture>("sprite.png");

    // Create the vertex array object and bind to it.
    // All subsequent VBO settings will be saved to this VAO.
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Create the vertex buffer object and bind to it.
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Copy the vertices into the buffer for OpenGL.
    float vertices[]
    {
        // position        // texCoord
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // top left 
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create the element buffer object and bind to it.
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

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

    // Enable blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);
}

void SpriteRenderer::update(float deltaTime)
{

}

void SpriteRenderer::render()
{
    // TODO: Replace this test rendering code.
    m_texture->bind();
    m_shader->use();
    glBindVertexArray(m_vao);

    glm::mat4 view{ glm::mat4(1.0f) };
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 proj{ glm::perspective(glm::radians(45.0f),
        (float)m_windowSize.x / m_windowSize.y, 0.1f, 100.0f) };

    for (auto it = m_models.begin(); it != m_models.end(); ++it)
    {
        glm::mat4 mvp = proj * view * (*it);
        m_shader->setMat4("mvp", mvp);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);

    m_models.clear();
}

void SpriteRenderer::addSprite(glm::mat4 model)
{
    m_models.push_back(model);
}

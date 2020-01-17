#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
    // TODO: Clean up this initialization code.
    m_shader = std::make_unique<Shader>("sprite.vs", "sprite.fs");
    m_texture = std::make_unique<Texture>("sprite.png");

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
}

void SpriteRenderer::update()
{
}

void SpriteRenderer::render()
{
    // TODO: Replace this test rendering code.
    m_texture->bind();
    m_shader->use();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

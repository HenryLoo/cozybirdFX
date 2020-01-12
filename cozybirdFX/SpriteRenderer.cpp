#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
    // TODO: Clean up this initialization code.
    m_shader = std::make_unique<Shader>("sprite.vs", "sprite.fs");

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
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void SpriteRenderer::update()
{
}

void SpriteRenderer::render()
{
    // TODO: Replace this test rendering code.
    m_shader->use();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

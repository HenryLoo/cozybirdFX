#include "SpriteRenderer.h"

#include <iostream>

namespace
{
    // TODO: Hard-coded passthrough shader, remove this later.
    const char* vertexShaderSource{ "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0" };

    const char* fragShaderSource{ "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}\0" };
}

SpriteRenderer::SpriteRenderer()
{
    // TODO: Clean up this initialization code.

    // Compile vertex shader.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    const int LOG_SIZE{ 512 };
    char infoLog[LOG_SIZE];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, LOG_SIZE, NULL, infoLog);
        std::cout << "SpriteRenderer: Failed to compile vertex shader.\n" << infoLog << std::endl;
    }

    // Compile fragment shader.
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, LOG_SIZE, NULL, infoLog);
        std::cout << "SpriteRenderer: Failed to compile fragment shader.\n" << infoLog << std::endl;
    }

    // Link the shader program.
    m_shaderProgram = glCreateProgram();

    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_shaderProgram, LOG_SIZE, NULL, infoLog);
        std::cout << "SpriteRenderer: Failed to link shader program.\n" << infoLog << std::endl;
    }

    // Clean up.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

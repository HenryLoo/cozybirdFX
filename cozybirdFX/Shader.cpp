#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace
{
    const std::string ASSET_PATH{ "assets/" };
    const std::string SHADER_PATH{ "shader/" };
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    // Compile vertex shader.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string shaderSrc;
    loadFromFile(ASSET_PATH + SHADER_PATH + vertexPath, shaderSrc);
    const char* shaderCSrc{ shaderSrc.c_str() };

    glShaderSource(vertexShader, 1, &shaderCSrc, NULL);
    glCompileShader(vertexShader);

    int success;
    const int LOG_SIZE{ 512 };
    char infoLog[LOG_SIZE];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, LOG_SIZE, NULL, infoLog);
        std::cout << "Shader: Failed to compile vertex shader.\n" << infoLog << std::endl;
    }

    // Compile fragment shader.
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    loadFromFile(ASSET_PATH + SHADER_PATH + fragmentPath, shaderSrc);
    shaderCSrc = shaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &shaderCSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, LOG_SIZE, NULL, infoLog);
        std::cout << "Shader: Failed to compile fragment shader.\n" << infoLog << std::endl;
    }

    // Link the shader program.
    m_id = glCreateProgram();

    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, LOG_SIZE, NULL, infoLog);
        std::cout << "Shader: Failed to link shader program.\n" << infoLog << std::endl;
    }

    // Clean up.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::use()
{
	glUseProgram(m_id);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::loadFromFile(const std::string& path, std::string& output) const
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Open the file.
        file.open(path);

        // Read the file buffer's contents into the stream.
        std::stringstream ss;
        ss << file.rdbuf();

        // Done with the file, so close it.
        file.close();

        // Get the string from the stream.
        output = ss.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Shader: Failed to load file." << std::endl;
    }
}

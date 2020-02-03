#include "ShaderLoader.h"
#include "Shader.h"

#include <glad/glad.h>

#include <iostream>

namespace
{
	const std::string SHADER_PATH{ "shader/" };
}

ShaderLoader::ShaderLoader() : ITypeLoader(SHADER_PATH)
{
}

std::shared_ptr<IAsset> ShaderLoader::interpretAsset(
    const std::vector<AssetBuffer> &data) const
{
    // Compile vertex shader.
    GLuint vertexShader{ glCreateShader(GL_VERTEX_SHADER) };
    std::string vertexSrc(data[0].buffer);
    vertexSrc = vertexSrc.substr(0, data[0].length);
    const char *vertexCSrc{ vertexSrc.c_str() };
    glShaderSource(vertexShader, 1, &vertexCSrc, NULL);
    glCompileShader(vertexShader);

    int success;
    const int LOG_SIZE{ 512 };
    char infoLog[LOG_SIZE];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, LOG_SIZE, NULL, infoLog);
        std::cout << "ShaderLoader: Failed to compile vertex shader.\n" << infoLog << std::endl;
    }

    // Compile fragment shader (optional).
    GLuint fragmentShader{ 0 };
    bool hasFragmentShader{ data[1].length > 0 };
    if (hasFragmentShader)
    {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        std::string fragSrc(data[1].buffer);
        fragSrc = fragSrc.substr(0, data[1].length);
        const char *fragCSrc{ fragSrc.c_str() };
        glShaderSource(fragmentShader, 1, &fragCSrc, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, LOG_SIZE, NULL, infoLog);
            std::cout << "ShaderLoader: Failed to compile fragment shader.\n" << infoLog << std::endl;
        }
    }

    // Compile geometry shader (optional).
    GLuint geometryShader{ 0 };
    bool hasGeometryShader{ data.size() == 3 };
    if (hasGeometryShader)
    {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        std::string geometrySrc(data[2].buffer);
        geometrySrc = geometrySrc.substr(0, data[2].length);
        const char *geometryCSrc{ geometrySrc.c_str() };
        glShaderSource(geometryShader, 1, &geometryCSrc, NULL);
        glCompileShader(geometryShader);

        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, LOG_SIZE, NULL, infoLog);
            std::cout << "ShaderLoader: Failed to compile geometry shader.\n" << infoLog << std::endl;
        }
    }

    std::shared_ptr<Shader> shader{ nullptr };
    if (success)
    {
        GLuint programId{ glCreateProgram() };
        shader = std::make_shared<Shader>(programId);

        shader->setVertexShader(vertexShader);
        if (hasFragmentShader)
            shader->setFragmentShader(fragmentShader);
        if (hasGeometryShader)
            shader->setGeometryShader(geometryShader);
    }

    return shader;
}

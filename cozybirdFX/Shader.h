#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Shader
{
public:
    // Create the shader program by linking the given vertex and fragment 
    // shaders.
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    // Use this shader program.
    void use();

    // Uniform setters.
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;

private:
    void loadFromFile(const std::string& path, std::string& output) const;

	// The shader program's id.
	GLuint m_id;
};
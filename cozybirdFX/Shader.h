#pragma once

#include "IAsset.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Shader : public IAsset
{
public:
    // Create the shader program by linking the given vertex and fragment 
    // shaders.
    Shader(unsigned int programId);
    ~Shader();

    // Use this shader program.
    void use();

    // Uniform setters.
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;

private:
	// The shader program's id.
	unsigned int m_programId;
};
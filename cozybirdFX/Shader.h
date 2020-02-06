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

    // Set shader ids.
    // This should be set before linking the program.
    void setVertexShader(unsigned int shaderId);
    void setGeometryShader(unsigned int shaderId);
    void setFragmentShader(unsigned int shaderId);

    // Get the program's id.
    // This should be used to make further configurations to the program
    // before linking.
    unsigned int getProgramId() const;

    // Link the shader program.
    void link();

    // Use this shader program.
    void use();

    // Uniform setters.
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string &name, glm::vec2 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec4(const std::string &name, glm::vec4 value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;

private:
	// The shader program's id.
	unsigned int m_programId;

    bool m_isLinked{ false };
    int m_vertexShaderId{ -1 };
    int m_geometryShaderId{ -1 };
    int m_fragmentShaderId{ -1 };
};
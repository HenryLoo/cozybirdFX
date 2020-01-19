#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(unsigned int programId) : m_programId(programId)
{

}

Shader::~Shader()
{
    glDeleteProgram(m_programId);
}

void Shader::use()
{
	glUseProgram(m_programId);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1,
        GL_FALSE, glm::value_ptr(value));
}
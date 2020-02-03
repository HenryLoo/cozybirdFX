#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Shader::Shader(unsigned int programId) : m_programId(programId)
{

}

Shader::~Shader()
{
    glDeleteProgram(m_programId);
}

void Shader::setVertexShader(unsigned int shaderId)
{
	if (m_vertexShaderId != -1)
		return;

	m_vertexShaderId = shaderId;
}

void Shader::setGeometryShader(unsigned int shaderId)
{
	if (m_geometryShaderId != -1)
		return;

	m_geometryShaderId = shaderId;
}

void Shader::setFragmentShader(unsigned int shaderId)
{
	if (m_fragmentShaderId != -1)
		return;

	m_fragmentShaderId = shaderId;
}

unsigned int Shader::getProgramId() const
{
	return m_programId;
}

void Shader::link()
{
	// Program is already linked.
	if (m_isLinked)
	{
		std::cout << "Shader::link: Shader program already linked." << std::endl;
		return;
	}

	m_isLinked = true;

	if (m_vertexShaderId != -1)
		glAttachShader(m_programId, m_vertexShaderId);

	if (m_geometryShaderId != -1)
		glAttachShader(m_programId, m_geometryShaderId);

	if (m_fragmentShaderId != -1)
		glAttachShader(m_programId, m_fragmentShaderId);

	glLinkProgram(m_programId);

	int success;
	const int LOG_SIZE{ 512 };
	char infoLog[LOG_SIZE];
	glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_programId, LOG_SIZE, NULL, infoLog);
		std::cout << "Shader::link: Failed to link shader program.\n" << infoLog << std::endl;
	}

	// Clean up.
	if (m_vertexShaderId != -1)
		glDeleteShader(m_vertexShaderId);
	if (m_geometryShaderId != -1)
		glDeleteShader(m_geometryShaderId);
	if (m_fragmentShaderId != -1)
		glDeleteShader(m_fragmentShaderId);
}

void Shader::use()
{	
	// Program is not linked.
	if (!m_isLinked)
	{
		std::cout << "Shader::use: Shader program has not been linked." << std::endl;
		return;
	}

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

void Shader::setVec3(const std::string &name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(m_programId, name.c_str()), 
		value.x, value.y, value.z);
}

void Shader::setVec4(const std::string &name, glm::vec4 value) const
{
	glUniform4f(glGetUniformLocation(m_programId, name.c_str()),
		value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1,
        GL_FALSE, glm::value_ptr(value));
}
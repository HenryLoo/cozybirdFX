#include "UIRenderer.h"
#include "AssetLoader.h"
#include "Shader.h"

#include <glad/glad.h>

UIRenderer::UIRenderer(AssetLoader *assetLoader)
{
	m_shader = assetLoader->load<Shader>({ "ui.vs", "ui.fs" });
	if (m_shader != nullptr)
		m_shader->link();

	// Create the vertex array object and bind to it.
	// All subsequent VBO settings will be saved to this VAO.
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Configure vertex buffer object with quad vertices.
	const GLfloat vertices[]
	{ 
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
	};

	glGenBuffers(1, &m_verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure element array buffer object.
	const GLuint elements[]
	{
		0, 1, 2,
		0, 3, 1
	};

	// Configure element buffer object.
	glGenBuffers(1, &m_verticesEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_verticesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
		GL_STATIC_DRAW);

	// Configure vertex attributes.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
		(GLvoid *)0);

	// Configure border buffer object.
	glGenBuffers(1, &m_borderVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_borderVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		(GLvoid *)0);
	glVertexAttribDivisorARB(1, 1);

	// Configure colour buffer object.
	glGenBuffers(2, &m_colourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourVBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
		(GLvoid *)0);
	glVertexAttribDivisorARB(2, 1);

	// Configure model buffer object.
	glGenBuffers(1, &m_modelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_modelVBO);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
		(GLvoid *)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
		(GLvoid *)sizeof(glm::vec4));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
		(GLvoid *)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
		(GLvoid *)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisorARB(3, 1);
	glVertexAttribDivisorARB(4, 1);
	glVertexAttribDivisorARB(5, 1);
	glVertexAttribDivisorARB(6, 1);

	// Unbind VBO and VAO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

UIRenderer::~UIRenderer()
{
	glDeleteBuffers(1, &m_verticesEBO);
	glDeleteBuffers(1, &m_verticesVBO);
	glDeleteBuffers(1, &m_borderVBO);
	glDeleteBuffers(1, &m_colourVBO);
	glDeleteBuffers(1, &m_modelVBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void UIRenderer::update(float deltaTime)
{
}

void UIRenderer::render()
{
	// Skip render if there are UI elements to draw.
	if (m_elements.empty())
		return;

	// Set blend mode.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set viewport.
	glViewport(0, 0, static_cast<GLsizei>(m_windowSize.x),
		static_cast<GLsizei>(m_windowSize.y));

	// Use orthographic projection, since we won't be needing perspective for UI.
	// This allows us to use vertex coordinates as screen coordinates.
	m_shader->use();
	m_shader->setMat4("projection", getOrthographicMatrix());

	// Instance vectors.
	std::vector<glm::vec3> borders;
	std::vector<glm::vec4> colours;
	std::vector<glm::mat4> modelMatrices;

	// Iterate through all UI elements to prepare values for rendering.
	for (const auto &thisElement : m_elements)
	{
		// Set the UI element's border.
		// The x, y members correspond to the UI element's width and height.
		// The z member is a flag to indicate whether the element has a border.
		borders.push_back(glm::vec3(thisElement.size.x, 
			thisElement.size.y, thisElement.hasBorder));

		// Set the UI element's colour.
		colours.push_back(thisElement.colour);

		// Prepare the model matrix for this character.
		glm::mat4 modelMatrix{ glm::mat4(1.0f) };
		modelMatrix = glm::translate(modelMatrix,
			glm::vec3(thisElement.pos.x, thisElement.pos.y, 0.f));
		modelMatrix = glm::scale(modelMatrix,
			glm::vec3(thisElement.size.x, thisElement.size.y, 1.f));
		modelMatrices.push_back(modelMatrix);
	}

	// Bind sizes.
	glBindBuffer(GL_ARRAY_BUFFER, m_borderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * borders.size(),
		&borders[0], GL_STATIC_DRAW);

	// Bind colours.
	glBindBuffer(GL_ARRAY_BUFFER, m_colourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colours.size(),
		&colours[0], GL_STATIC_DRAW);

	// Bind model instances.
	glBindBuffer(GL_ARRAY_BUFFER, m_modelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelMatrices.size(),
		&modelMatrices[0], GL_STATIC_DRAW);

	// Unbind VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Draw UI elements.
	glBindVertexArray(m_VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(modelMatrices.size()));

	// Unbind VAO.
	glBindVertexArray(0);

	// Unbind texture.
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::list<UIRenderer::Properties>::iterator UIRenderer::addElement(const Properties &prop)
{
	m_elements.push_back(prop);

	// Return the iterator the element that was just added.
	return std::prev(std::end(m_elements));
}

void UIRenderer::clearElements()
{
	m_elements.clear();
}

#include "TextRenderer.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "Font.h"
#include "Shader.h"

#include <glad/glad.h>

TextRenderer::TextRenderer(AssetLoader &assetLoader)
{
    m_shader = assetLoader.load<Shader>({ "text.vs", "text.fs" });
    if (m_shader != nullptr)
        m_shader->link();

	m_font = assetLoader.load<Font>("default.ttf", 16);

    // Create the vertex array object and bind to it.
    // All subsequent VBO settings will be saved to this VAO.
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Configure vertex buffer object with quad vertices.
    const GLfloat vertices[]
    {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
		(GLvoid *)0);

	// Configure clip buffer object.
	glGenBuffers(1, &m_clipVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_clipVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
		(GLvoid *)0);
	glVertexAttribDivisorARB(1, 1);

	// Configure colour buffer object.
	glGenBuffers(1, &m_colourVBO);
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

TextRenderer::~TextRenderer()
{
	glDeleteBuffers(1, &m_verticesEBO);
	glDeleteBuffers(1, &m_verticesVBO);
	glDeleteBuffers(1, &m_clipVBO);
	glDeleteBuffers(1, &m_colourVBO);
	glDeleteBuffers(1, &m_modelVBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void TextRenderer::render(float deltaTime, const Camera &camera)
{
	// Skip render if there is no text to draw.
	if (m_texts.empty())
		return;

	// Set blend mode.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use orthographic projection, since we won't be needing perspective for text.
	// This allows us to use vertex coordinates as screen coordinates.
	m_shader->use();
	m_shader->setMat4("projection", camera.getUIProjection());

	// Iterate through in-use fonts.
	for (auto it = m_texts.begin(); it != m_texts.end(); ++it)
	{
		Font *thisFont{ it->first };
		const std::list<Properties> &thisTexts{ it->second };

		// Instance vectors.
		std::vector<glm::vec4> clips;
		std::vector<glm::vec4> colours;
		std::vector<glm::mat4> modelMatrices;

		// Get font attributes.
		const std::vector<Font::Glyph> &chars{ thisFont->getGlyphs() };
		const std::vector<Font::Clip> &fontClips{ thisFont->getClips() };
		const glm::ivec2 textureSize{ thisFont->getWidth(), thisFont->getHeight() };

		// Iterate through all text strings using this font.
		for (const Properties &thisText : thisTexts)
		{
			// Skip disabled elements.
			if (!thisText.isEnabled)
				continue;

			// Iterate through each char in the text string to get its
			// bounding width and height.
			std::string::const_iterator c;
			float alignOffsetX{ 0.f };
			if (thisText.align != TextAlign::LEFT)
			{
				float textWidth{ 0.f };
				for (c = thisText.text.begin(); c != thisText.text.end(); ++c)
				{
					// The current character.
					const Font::Glyph &character{ chars[*c] };

					// Add to total text width.
					textWidth += (character.advance >> 6) *thisText.scale;
				}

				// Calculate alignment offsets.
				if (thisText.align == TextAlign::CENTER)
				{
					alignOffsetX = (thisText.size.x - textWidth) / 2;
				}
				else if (thisText.align == TextAlign::RIGHT)
				{
					alignOffsetX = thisText.size.x - textWidth;
				}
			}

			// Iterate through each char in the text string to construct
			// its instance data.
			int index{ 0 };
			float x{ thisText.pos.x + alignOffsetX };
			for (c = thisText.text.begin(); c != thisText.text.end(); ++c)
			{
				// The current character.
				const Font::Glyph &character{ chars[*c] };

				// Set the character's clip.
				const Font::Clip &clip{ fontClips[*c] };
				clips.push_back(glm::vec4(
					static_cast<float>(clip.position.x) / textureSize.x,
					static_cast<float>(clip.position.y) / textureSize.y,
					static_cast<float>(clip.size.x) / textureSize.x,
					static_cast<float>(clip.size.y) / textureSize.y
				));

				// Set the character's colour.
				colours.push_back(thisText.colour);

				// Set positions.
				// Glyphs are already vertically aligned when constructing the texture 
				// atlas, so bearing.y is unnecessary.
				GLfloat posX{ x + character.bearing.x * thisText.scale };
				GLfloat posY{ thisText.pos.y * thisText.scale };

				if (thisText.isVerticalCenter)
				{
					posY += ((thisText.size.y - clip.size.y) / 2.f * thisText.scale);
					posY = roundf(posY); // removes some artifacts
				}

				// Set sizes.
				GLfloat w{ clip.size.x * thisText.scale };
				GLfloat h{ clip.size.y * thisText.scale };

				// Prepare the model matrix for this character.
				glm::mat4 modelMatrix{ glm::mat4(1.0f) };
				modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, posY, 0.f));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(w, h, 1.f));
				modelMatrices.push_back(modelMatrix);

				// Move the cursor forward for the next glyph.
				// Bitshift by 6 to get the value in pixels, since ( 2^6 = 64 and the 
				// value is measured in 1/64th of a pixel ).
				x += (character.advance >> 6) *thisText.scale;

				++index;
			}
		}

		// Bind clips.
		glBindBuffer(GL_ARRAY_BUFFER, m_clipVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * clips.size(),
			&clips[0], GL_STATIC_DRAW);

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

		// Bind to the font texture atlas.
		glActiveTexture(GL_TEXTURE0);
		thisFont->bind();

		// Draw text.
		glBindVertexArray(m_VAO);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(modelMatrices.size()));

		// Unbind VAO.
		glBindVertexArray(0);

		// Unbind texture.
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

std::list<TextRenderer::Properties>::iterator TextRenderer::addText(const Properties &prop, Font *font)
{
	// Use the default font if no font provided.
	if (font == nullptr)
		font = m_font.get();

	auto it{ m_texts.find(font) };

	// This font is already being used by some other text.
	if (it != m_texts.end())
	{
		it->second.push_back(prop);
		
		// Return the iterator the element that was just added.
		return std::prev(std::end(it->second));
	}
	// No text already using this font.
	else
	{
		auto result{ m_texts.insert({ font, std::list<Properties>{ prop } }) };

		// Return the iterator the element that was just added.
		return std::prev(std::end(result.first->second));
	}
}

void TextRenderer::clearText()
{
	m_texts.clear();
}

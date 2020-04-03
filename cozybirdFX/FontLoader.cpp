#include "FontLoader.h"
#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>

#include <iostream>

namespace
{
	const std::string FONT_PATH{ "font/" };

	const int NUM_CHARS_PER_ROW{ 16 };
	const int NUM_CHARS_PER_COL{ 16 };
	const int TOTAL_NUM_CHARS{ NUM_CHARS_PER_ROW * NUM_CHARS_PER_COL };
}

FontLoader::FontLoader() : ITypeLoader(FONT_PATH)
{
	if (FT_Init_FreeType(&m_library))
	{
		std::cout << "FontLoader: Failed to initialize FreeType library" << std::endl;
	}
}

FontLoader::~FontLoader()
{
	FT_Done_FreeType(m_library);
}

std::shared_ptr<IAsset> FontLoader::interpretAsset(
	const std::vector<AssetBuffer> &data, int flag) const
{
	// Invalid font size specified in flag parameter.
	if (flag <= 0)
	{
		std::cout << "FontLoader::interpretAsset: Invalid font size" << std::endl;
		return nullptr;
	}

	// Load font from memory.
	FT_Face face;
	if (FT_New_Memory_Face(m_library, (unsigned char *)data[0].buffer, 
		data[0].length, 0, &face))
	{
		std::cout << "FontLoader::interpretAsset: Failed to load font from memory" << std::endl;
		return nullptr;
	}

	// Set face size with the flag parameter's value.
	if (FT_Set_Pixel_Sizes(face, 0, flag))
	{
		std::cout << "FontLoader::interpretAsset: Failed to set font size" << std::endl;
		return nullptr;
	}

	// Set alignment requirement to 1.
	// This affects how pixel data is read from memory.
	// The default value is 4, but we are only using 1 byte per pixel (GL_RED).
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate the glyphs for this face.
	GLuint cellWidth{ 0 };
	int maxBearing{ 0 };
	int minHang{ 0 };
	std::vector<FT_Glyph_Metrics> metrics;
	std::vector<Font::Glyph> glyphs;
	std::vector<GLubyte *> charPixels;

	metrics.resize(TOTAL_NUM_CHARS);
	for (int i = 0; i < TOTAL_NUM_CHARS; ++i)
	{
		// Load the char glyph.
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			std::cout << "FontLoader::interpretAsset: Failed to load character glyph: << " << i << std::endl;
			continue;
		}

		// Get the metrics.
		metrics[i] = face->glyph->metrics;

		// Calculate max bearing.
		// Divide by 64, since values are stored in 1/64 pixels.
		if (metrics[i].horiBearingY / 64 > maxBearing)
		{
			maxBearing = metrics[i].horiBearingY / 64;
		}

		// Calculate max width.
		if (static_cast<GLuint>(metrics[i].width / 64) > cellWidth)
		{
			cellWidth = metrics[i].width / 64;
		}

		// Calculate glyph hang.
		int glyphHang{ (metrics[i].horiBearingY - metrics[i].height) / 64 };
		if (glyphHang < minHang)
		{
			minHang = glyphHang;
		}

		// Store the character.
		GLuint width{ face->glyph->bitmap.width };
		GLuint height{ face->glyph->bitmap.rows };
		Font::Glyph character
		{
			glm::ivec2(width, height),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		glyphs.push_back(character);

		// Store the character's pixels.
		GLuint size{ width * height };
		GLubyte *pixels{ new GLubyte[size] };
		memcpy(pixels, face->glyph->bitmap.buffer, size);
		charPixels.push_back(pixels);
	}

	// Create bitmap font.
	GLuint cellHeight{ static_cast<GLuint>(maxBearing - minHang) };
	GLuint textureW{ cellWidth * NUM_CHARS_PER_COL };
	GLuint textureH{ cellHeight * NUM_CHARS_PER_ROW };
	GLuint size{ textureW * textureH };
	GLubyte *texturePixels{ new GLubyte[size] };
	memset(texturePixels, 0, size);

	// Begin creating bitmap font.
	GLuint currentChar{ 0 };

	// Prepare the texture clip.
	Font::Clip nextClip;
	std::vector<Font::Clip> clips;

	// Blitting coordinates.
	int bX{ 0 };
	int bY{ 0 };

	// Go through cell rows.
	for (int rows = 0; rows < NUM_CHARS_PER_ROW; rows++)
	{
		// Go through each cell column in the row.
		for (int cols = 0; cols < NUM_CHARS_PER_COL; cols++)
		{
			// Set base offsets.
			bX = cellWidth * cols;
			bY = cellHeight * rows;

			// Initialize clip.
			nextClip.position.x = bX;
			nextClip.position.y = bY;
			nextClip.size.x = metrics[currentChar].width / 64;
			nextClip.size.y = cellHeight;

			// Blit character.
			const Font::Glyph &ch{ glyphs[currentChar] };
			GLint x{ bX };
			GLint y{ bY + maxBearing - (metrics[currentChar].horiBearingY / 64) };
			for (int i = 0; i < ch.size.y; ++i)
			{
				memcpy(&texturePixels[(i + y) * textureW + x],
					&charPixels[currentChar][i * ch.size.x], ch.size.x);
			}

			// Go to the next character.
			clips.push_back(nextClip);
			++currentChar;
		}
	}

	// Generate font texture.
	// Use GL_RED as the texture's internalFormat and format arguments,
	// since the generated bitmap is a grayscale 8-bit image.
	// Each colour is represented by 1 byte.
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		textureW,
		textureH,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		texturePixels
	);

	// Set texture options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	// Release pixels.
	delete[] texturePixels;
	for (const GLubyte *pixels : charPixels)
	{
		delete[] pixels;
	}

	// Clean up.
	FT_Done_Face(face);

	// Instantiate the font.
	std::shared_ptr<Font> font{ std::make_shared<Font>(textureId, textureW,
		textureH, glyphs, clips) };

	return font;
}
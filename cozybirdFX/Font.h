#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

#include <vector>

class Font : public Texture
{
public:
	// Metrics for a character.
	struct Glyph
	{
		// The width/height of the character.
		glm::ivec2 size;

		// Offset from baseline to top/left.
		glm::ivec2 bearing;

		// The horizontal distance between this glyph and the next.
		// This value is stored in 1/64 pixels.
		unsigned int advance;
	};

	struct Clip
	{
		// x, y positions.
		glm::ivec2 position;

		// The width/height.
		glm::ivec2 size;
	};

	Font(unsigned int textureId, int textureWidth, int textureHeight, 
		const std::vector<Glyph> &glyphs, const std::vector<Clip> &clips);

	~Font();

	// Getter functions.
	const std::vector<Glyph> &getGlyphs() const;
	const std::vector<Clip> &getClips() const;

private:
	// Hold all glyphs for this font.
	const std::vector<Glyph> m_glyphs;

	// Hold all clips for this font.
	const std::vector<Clip> m_clips;
};
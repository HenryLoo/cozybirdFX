#include "Font.h"

Font::Font(unsigned int textureId, int textureWidth, int textureHeight,
	const std::vector<Glyph> &glyphs, const std::vector<Clip> &clips) :
	Texture(textureId, textureWidth, textureHeight, 1),
	m_glyphs(glyphs), m_clips(clips)
{
}

Font::~Font()
{
}

const std::vector<Font::Glyph> &Font::getGlyphs() const
{
	return m_glyphs;
}

const std::vector<Font::Clip> &Font::getClips() const
{
	return m_clips;
}

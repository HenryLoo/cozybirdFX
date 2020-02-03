#pragma once

#include "IAsset.h"

class Texture : public IAsset
{
public:
	Texture(unsigned int textureId, int width, int height, int numChannels);
	~Texture();

	// Bind to this texture.
	void bind() const;

private:
	// This texture's id.
	unsigned int m_textureId{ 0 };

	// Texture properties.
	int m_width{ 0 };
	int m_height{ 0 };
	int m_numChannels{ 0 };
};
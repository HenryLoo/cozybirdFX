#pragma once

#include "ITypeLoader.h"

class TextureLoader : public ITypeLoader
{
public:
	TextureLoader();

protected:
	virtual std::shared_ptr<IAsset> interpretAsset(char *dataBuffer, int bufferLength) const;
};
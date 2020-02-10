#pragma once

#include "ITypeLoader.h"

class TextureLoader : public ITypeLoader
{
public:
	TextureLoader();

protected:
	virtual std::shared_ptr<IAsset> interpretAsset(
		const std::vector<AssetBuffer> &data, int flag) const;
};
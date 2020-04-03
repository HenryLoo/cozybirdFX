#pragma once

#include "ITypeLoader.h"

class ShaderLoader : public ITypeLoader
{
public:
	ShaderLoader();

protected:
	// First buffer is for vertex shader.
	// Second buffer is for fragment shader.
	virtual std::shared_ptr<IAsset> interpretAsset(
		const std::vector<AssetBuffer> &data, int flag) const override;
};
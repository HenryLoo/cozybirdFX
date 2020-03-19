#pragma once

#include "ITypeLoader.h"

struct FT_LibraryRec_;

class FontLoader : public ITypeLoader
{
public:
	FontLoader();
	~FontLoader();

protected:
	// Flag specifies font size.
	virtual std::shared_ptr<IAsset> interpretAsset(
		const std::vector<AssetBuffer> &data, int flag) const override;

private:
	FT_LibraryRec_ *m_library{ nullptr };
};
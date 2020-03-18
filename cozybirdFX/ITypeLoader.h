#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

class IAsset;

class ITypeLoader
{
public:
	ITypeLoader(const std::string &typePath);

	// Load an asset into a byte array, given its file path.
	// After loading, try to interpret the asset using the
	// specific loader's interpretAsset implementation.
	// The flag is an optional parameter that may be used by specific loaders.
	std::shared_ptr<IAsset> load(
		const std::initializer_list<std::string> &fileNames, int flag, 
		bool isAbsPath);

protected:
	struct AssetBuffer
	{
		char *buffer;
		int length;
	};

	// Interpret the loaded byte array into a specific asset type.
	// The specific loader subclass should implement this.
	virtual std::shared_ptr<IAsset> interpretAsset(
		const std::vector<AssetBuffer> &data, int flag) const = 0;

private:
	// Load an asset from the cache if it exists.
	std::shared_ptr<IAsset> loadCached(const std::string &fileName);

	// Cache an asset.
	void cache(const std::string &fileName, std::shared_ptr<IAsset> asset);

	// Hold all cached assets.
	std::unordered_map<std::string, std::shared_ptr<IAsset>> m_cachedAssets;

	// The specific asset type's folder name.
	std::string m_typePath;
};
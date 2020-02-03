#include "ITypeLoader.h"
#include "IAsset.h"

#include <iostream>

namespace
{
	const std::string ASSET_PATH{ "assets/" };
}

ITypeLoader::ITypeLoader(const std::string &typePath) :
	m_typePath(typePath)
{

}

std::shared_ptr<IAsset> ITypeLoader::load(const std::initializer_list<std::string> &fileNames)
{
	// First, check cache for this asset.
	const std::string assetName{ *fileNames.begin() };
	std::shared_ptr<IAsset> cached{ loadCached(assetName) };
	if (cached != nullptr)
		return cached;

	std::vector<AssetBuffer> data;
	for (const std::string &fileName : fileNames)
	{
		// Skip if no file name.
		if (fileName.empty())
		{
			data.push_back({});
			continue;
		}

		// Get the file path.
		const std::string filePath{ ASSET_PATH + m_typePath + fileName };

		// If asset was not found in cache, then load it.
		std::ifstream stream(filePath, std::ifstream::binary);
		if (!stream.is_open())
		{
			std::cout << "ITypeLoader::load: Failed to open file in stream '"
				<< filePath << "'" << std::endl;
		}

		// Get the file size.
		stream.seekg(0, stream.end);
		int length{ static_cast<int>(stream.tellg()) };
		stream.seekg(0, stream.beg);

		// Read the data from the stream into the buffer.
		char *buffer{ new char[length] };
		stream.read(buffer, length);
		stream.close();

		data.push_back({ buffer, length });

		std::cout << "ITypeLoader::load: '" << filePath << "', size: " << length << std::endl;
	}

	// Interpret the asset.
	std::shared_ptr<IAsset> asset{ interpretAsset(data) };

	// Clean up.
	for (auto &item : data)
		delete[] item.buffer;

	// Cache the loaded asset.
	cache(assetName, asset);

	return asset;
}

std::shared_ptr<IAsset> ITypeLoader::loadCached(const std::string &fileName)
{
	auto it{ m_cachedAssets.find(fileName) };
	if (it == m_cachedAssets.end())
	{
		return nullptr;
	}

	return it->second;
}

void ITypeLoader::cache(const std::string &fileName, std::shared_ptr<IAsset> asset)
{
	m_cachedAssets.insert({ fileName, asset });
}

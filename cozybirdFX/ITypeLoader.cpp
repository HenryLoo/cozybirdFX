#include "ITypeLoader.h"
#include "IAsset.h"

#include <iostream>

namespace
{
	const std::string ASSET_PATH{ "assets/" };

	// The maximum duration that an asset can remain in the cache, in seconds.
	const float MAX_CACHE_DURATION{ 300.f };
}

ITypeLoader::ITypeLoader(const std::string &typePath) :
	m_typePath(typePath)
{

}

void ITypeLoader::update(float deltaTime)
{
	for (auto it = m_cachedAssets.begin(); it != m_cachedAssets.end();)
	{
		// If the only instance of using this asset is from this cache, then
		// start timing it for deletion.
		if (it->second.asset.use_count() == 1)
			it->second.elapsedTime += deltaTime;
		// Reset the timer as long as there is another object using this asset.
		else
			it->second.elapsedTime = 0.f;

		// Delete this asset from the cache if its elapsed time exceeds the max
		// cache duration.
		if (it->second.elapsedTime >= MAX_CACHE_DURATION)
		{
			std::cout << "ITypeLoader::update: '" << it->first << "' deleted from cache." << std::endl;
			it = m_cachedAssets.erase(it);
		}
		else
		{
			++it;
		}
	}
}

std::shared_ptr<IAsset> ITypeLoader::load(const std::initializer_list<std::string> &fileNames, 
	int flag, bool isAbsPath)
{
	// Get the asset's name.
	std::string flagStr{ "" };
	if (flag)
		flagStr = "_" + std::to_string(flag);
	const std::string assetName{ *fileNames.begin() + flagStr };

	// First, check cache for this asset.
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
		std::string filePath{ fileName };
		if (!isAbsPath)
			filePath = ASSET_PATH + m_typePath + filePath;

		// If asset was not found in cache, then load it.
		std::ifstream stream(filePath, std::ifstream::binary);
		if (!stream.is_open())
		{
			std::cout << "ITypeLoader::load: Failed to open file in stream '"
				<< filePath << "'" << std::endl;
			return nullptr;
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
	std::shared_ptr<IAsset> asset{ interpretAsset(data, flag) };

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

	return it->second.asset;
}

void ITypeLoader::cache(const std::string &fileName, std::shared_ptr<IAsset> asset)
{
	m_cachedAssets.insert({ fileName, { asset, 0.f } });
}

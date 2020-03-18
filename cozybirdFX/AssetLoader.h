#pragma once

#include "ITypeLoader.h"

#include <iostream>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

class IAsset;

class AssetLoader
{
public:
	// Load an asset through its specific TypeLoader, given its type as 
	// the template.
	template<typename T>
	std::shared_ptr<T> load(const std::string &fileName, 
		int flag = 0, bool isAbsPath = false);
	template<typename T>
	std::shared_ptr<T> load(const std::initializer_list<std::string> &fileNames, 
		int flag = 0, bool isAbsPath = false);

	// Register a TypeLoader to use for loading assets of a specific asset 
	// type. The asset type is specified in the template.
	template<typename T>
	void registerLoader(ITypeLoader *loader);

private:
	// Map asset type to registered TypeLoaders.
	std::unordered_map<std::type_index, std::unique_ptr<ITypeLoader>> m_loaders;
};

template<typename T>
inline std::shared_ptr<T> AssetLoader::load(const std::string &fileName, 
	int flag, bool isAbsPath)
{
	return load<T>({ fileName }, flag, isAbsPath);
}

template<typename T>
inline std::shared_ptr<T> AssetLoader::load(const std::initializer_list<std::string> &fileNames, 
	int flag, bool isAbsPath)
{
	const std::string fileName{ *fileNames.begin() };

	// Get the TypeLoader for this asset type.
	std::type_index type{ std::type_index(typeid(T)) };
	auto loaderIt{ m_loaders.find(type) };
	if (loaderIt == m_loaders.end())
	{
		std::cout << "AssetLoader::load: Could not find TypeLoader for '" 
			<< fileName << "'" << std::endl;
		return nullptr;
	}

	// Use the TypeLoader to load the asset.
	std::shared_ptr<IAsset> asset{ loaderIt->second->load(fileNames, flag, 
		isAbsPath) };
	if (asset == nullptr)
	{
		std::cout << "AssetLoader::load: Could not load '"
			<< fileName << "'" << std::endl;
		return nullptr;
	}

	return std::dynamic_pointer_cast<T>(asset);
}

template<typename T>
inline void AssetLoader::registerLoader(ITypeLoader *loader)
{
	std::type_index type{ std::type_index(typeid(T)) };
	m_loaders.insert({ type, std::unique_ptr<ITypeLoader>(loader) });
}

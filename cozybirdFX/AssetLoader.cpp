#include "AssetLoader.h"

void AssetLoader::update(float deltaTime)
{
	for (auto &loader : m_loaders)
		loader.second->update(deltaTime);
}

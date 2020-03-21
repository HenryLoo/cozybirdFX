#pragma once

#include "IMenuState.h"

class FileState : public IMenuState
{
public:
	FileState(Engine &engine, AssetLoader &assetLoader, 
		EmitterRenderer &eRenderer);

private:
	virtual void initMenu() override;

	// UI elements.
	std::shared_ptr<UIButton> m_newButton{ nullptr };
	std::shared_ptr<UIButton> m_loadButton{ nullptr };
	std::shared_ptr<UIButton> m_saveButton{ nullptr };
};
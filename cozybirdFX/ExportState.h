#pragma once

#include "IMenuState.h"

class ExportState : public IMenuState
{
public:
	ExportState(Engine &engine, AssetLoader &assetLoader,
		EmitterRenderer &eRenderer);

private:
	virtual void initMenu() override;

	// UI elements.
	std::shared_ptr<UIButton> m_sheetButton{ nullptr };
	std::shared_ptr<UIButton> m_gifButton{ nullptr };
};
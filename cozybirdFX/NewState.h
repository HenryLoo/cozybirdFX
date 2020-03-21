#pragma once

#include "IMenuState.h"

class NewState : public IMenuState
{
public:
	NewState(Engine &engine, AssetLoader &assetLoader,
		EmitterRenderer &eRenderer);

private:
	virtual void initMenu() override;

	// UI elements.
	std::shared_ptr<UIButton> m_defaultButton{ nullptr };
	std::shared_ptr<UIButton> m_fireButton{ nullptr };
	std::shared_ptr<UIButton> m_smokeButton{ nullptr };
};
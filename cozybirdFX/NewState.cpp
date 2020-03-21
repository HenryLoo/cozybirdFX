#include "NewState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "UIButton.h"

NewState::NewState(Engine &engine, AssetLoader &assetLoader, 
	EmitterRenderer &eRenderer) :
	IMenuState(engine, assetLoader, eRenderer, "Select a preset to generate...")
{

}

void NewState::initMenu()
{
	Engine &engine{ m_engine };
	AssetLoader &assetLoader{ m_assetLoader };
	EmitterRenderer &eRenderer{ m_eRenderer };

	m_defaultButton = std::make_shared<UIButton>("Default",
		BUTTON_SIZE, false, [&engine, &assetLoader, &eRenderer]()
		{
			eRenderer.init(assetLoader);
			engine.popState();
			engine.popState();
		});

	addButton(m_defaultButton);

	m_fireButton = std::make_shared<UIButton>("Fire",
		BUTTON_SIZE, false, [&engine, &assetLoader, &eRenderer]()
		{
			eRenderer.init(assetLoader);

			// TODO;

			engine.popState();
			engine.popState();
		});

	addButton(m_fireButton);

	m_smokeButton = std::make_shared<UIButton>("Smoke",
		BUTTON_SIZE, false, [&engine, &assetLoader, &eRenderer]()
		{
			eRenderer.init(assetLoader);

			// TODO;

			engine.popState();
			engine.popState();
		});

	addButton(m_smokeButton);
}
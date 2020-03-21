#include "ExportState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "UIButton.h"

#include <nfd/nfd.h>

ExportState::ExportState(Engine &engine, AssetLoader &assetLoader,
	EmitterRenderer &eRenderer) :
	IMenuState(engine, assetLoader, eRenderer, "Export Menu")
{

}

void ExportState::initMenu()
{
	Engine &engine{ m_engine };
	AssetLoader &assetLoader{ m_assetLoader };
	EmitterRenderer &eRenderer{ m_eRenderer };

	m_sheetButton = std::make_shared<UIButton>("Export as spritesheet...",
		BUTTON_SIZE, false, [&engine, &eRenderer]()
		{
			nfdchar_t *path{ nullptr };
			nfdresult_t result = NFD_SaveDialog("png", nullptr, &path);

			// Save if path is valid.
			if (result == NFD_OKAY)
			{
				glm::vec2 windowSize{ engine.getWindowSize() };
				eRenderer.exportSpriteSheet(windowSize, path);
				engine.popState();
			}
			else if (result != NFD_CANCEL)
			{
				std::cout << "ExportState, save path error: " <<
					NFD_GetError() << std::endl;
			}
		});

	addButton(m_sheetButton);

	m_gifButton = std::make_shared<UIButton>("Export as gif...",
		BUTTON_SIZE, false, [&engine, &eRenderer]()
		{
			nfdchar_t *path{ nullptr };
			nfdresult_t result = NFD_SaveDialog("gif", nullptr, &path);

			// Save if path is valid.
			if (result == NFD_OKAY)
			{
				glm::vec2 windowSize{ engine.getWindowSize() };
				eRenderer.exportGif(windowSize, path);
				engine.popState();
			}
			else if (result != NFD_CANCEL)
			{
				std::cout << "ExportState, save path error: " <<
					NFD_GetError() << std::endl;
			}
		});

	addButton(m_gifButton);
}
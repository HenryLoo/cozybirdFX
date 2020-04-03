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

	m_sheetButton = std::make_shared<UIButton>("Export as sprite sheet...",
		BUTTON_SIZE, false, [&engine, &eRenderer]()
		{
			nfdchar_t *path{ nullptr };
			const std::string FILE_FORMAT{ ".png" };
			nfdresult_t result = NFD_SaveDialog(FILE_FORMAT.substr(1).c_str(), nullptr, &path);

			// Save if path is valid.
			if (result == NFD_OKAY)
			{
				glm::vec2 windowSize{ engine.getWindowSize() };
				std::string filePath(path);
				free(path);
				getFilePath(filePath, FILE_FORMAT);
				eRenderer.exportSpriteSheet(engine, filePath);
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
			const std::string FILE_FORMAT{ ".gif" };
			nfdresult_t result = NFD_SaveDialog(FILE_FORMAT.substr(1).c_str(), nullptr, &path);

			// Save if path is valid.
			if (result == NFD_OKAY)
			{
				glm::vec2 windowSize{ engine.getWindowSize() };
				std::string filePath(path);
				free(path);
				getFilePath(filePath, FILE_FORMAT);
				eRenderer.exportGif(engine, filePath);
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
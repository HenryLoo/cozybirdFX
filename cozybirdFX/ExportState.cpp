#include "ExportState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "TextRenderer.h"
#include "UIButton.h"
#include "UIRenderer.h"
#include "UIText.h"

#include <nfd/nfd.h>

namespace
{
	const glm::vec2 BUTTON_SIZE{ 500.f, 80.f };
}

ExportState::ExportState(Engine &engine, AssetLoader &assetLoader, 
	EmitterRenderer &eRenderer)
{
	m_tRenderer = std::make_shared<TextRenderer>(assetLoader);
	m_uRenderer = std::make_shared<UIRenderer>(assetLoader);

	glm::vec2 windowSize{ engine.getWindowSize() };
	m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
		windowSize);

	m_title = std::make_shared<UIText>("Export Menu", BUTTON_SIZE);
	m_panel->addElement(m_title);

	m_panel->addNewLine();
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
	m_panel->addElement(m_sheetButton);

	m_panel->addNewLine();
	m_gifButton = std::make_shared<UIButton>("Export as gif...",
		BUTTON_SIZE, false, [&engine, &eRenderer]()
		{
			nfdchar_t *path{ nullptr };
			nfdresult_t result = NFD_SaveDialog("json", nullptr, &path);

			// Save if path is valid.
			if (result == NFD_OKAY)
			{
			}
			else if (result != NFD_CANCEL)
			{
				std::cout << "ExportState, save path error: " <<
					NFD_GetError() << std::endl;
			}
		});
	m_panel->addElement(m_gifButton);

	m_panel->addNewLine();
	m_closeButton = std::make_shared<UIButton>("Close",
		BUTTON_SIZE, false, [&engine]()
		{
			engine.popState();
		});
	m_panel->addElement(m_closeButton);

	m_panel->addToRenderer(*m_uRenderer, *m_tRenderer);
	m_title->setAlign(TextRenderer::TextAlign::CENTER);
}

void ExportState::handleInput(InputManager &inputManager)
{
	m_panel->process(inputManager);
}

void ExportState::render(float deltaTime, const Camera &camera)
{
	m_uRenderer->render(deltaTime, camera);
	m_tRenderer->render(deltaTime, camera);
}

void ExportState::enter()
{
}

void ExportState::update(float deltaTime)
{
}

void ExportState::resize(Camera &camera)
{
	m_panel->setSize(m_windowSize);

	glm::vec2 buttonPos{ (m_panel->getSize().x - BUTTON_SIZE.x) / 2.f, -1.f };
	m_title->setPosition(buttonPos);
	m_sheetButton->setPosition(buttonPos);
	m_gifButton->setPosition(buttonPos);
	m_closeButton->setPosition(buttonPos);
}

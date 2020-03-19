#include "FileState.h"
#include "Engine.h"
#include "TextRenderer.h"
#include "UIButton.h"
#include "UIRenderer.h"
#include "UIText.h"

namespace
{
	const glm::vec2 BUTTON_SIZE{ 500.f, 80.f };
}

FileState::FileState(Engine &engine, AssetLoader &assetLoader)
{
	m_tRenderer = std::make_shared<TextRenderer>(assetLoader);
	m_uRenderer = std::make_shared<UIRenderer>(assetLoader);

	glm::vec2 windowSize{ engine.getWindowSize() };
	m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
		windowSize);

	m_title = std::make_shared<UIText>("File Menu", BUTTON_SIZE);
	m_panel->addElement(m_title);

	m_panel->addNewLine();
	m_loadButton = std::make_shared<UIButton>("Load",
		BUTTON_SIZE, false, []()
		{
			// TODO;
		});
	m_panel->addElement(m_loadButton);

	m_panel->addNewLine();
	m_saveButton = std::make_shared<UIButton>("Save",
		BUTTON_SIZE, false, []()
		{
			// TODO;
		});
	m_panel->addElement(m_saveButton);

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

void FileState::handleInput(InputManager &inputManager)
{
	m_panel->process(inputManager);
}

void FileState::render(float deltaTime, const Camera &camera)
{
	m_uRenderer->render(deltaTime, camera);
	m_tRenderer->render(deltaTime, camera);
}

void FileState::update(float deltaTime)
{

}

void FileState::resize(Camera &camera)
{
	m_panel->setSize(m_windowSize);

	glm::vec2 buttonPos{ (m_panel->getSize().x - BUTTON_SIZE.x) / 2.f, -1.f };
	m_title->setPosition(buttonPos);
	m_loadButton->setPosition(buttonPos);
	m_saveButton->setPosition(buttonPos);
	m_closeButton->setPosition(buttonPos);
}

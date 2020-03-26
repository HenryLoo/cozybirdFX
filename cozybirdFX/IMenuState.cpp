#include "IMenuState.h"
#include "Engine.h"
#include "UIButton.h"
#include "UIText.h"

const glm::vec2 IMenuState::BUTTON_SIZE{ 500.f, 80.f };

IMenuState::IMenuState(Engine &engine, AssetLoader &assetLoader, 
	EmitterRenderer &eRenderer, const std::string &title) :
	m_engine(engine), m_assetLoader(assetLoader), m_eRenderer(eRenderer)
{
	m_tRenderer = std::make_shared<TextRenderer>(assetLoader);
	m_uRenderer = std::make_shared<UIRenderer>(assetLoader);

	glm::vec2 windowSize{ engine.getWindowSize() };
	m_panel = std::make_unique<UIContainer>(glm::vec2(0.f, 0.f),
		windowSize);

	// Add menu title.
	m_title = std::make_shared<UIText>(title, BUTTON_SIZE);
	m_panel->addElement(m_title);
	m_panel->addNewLine();
}

void IMenuState::handleInput(InputManager &inputManager)
{
	m_panel->process(inputManager);
}

void IMenuState::render(float deltaTime, const Camera &camera)
{
	m_uRenderer->render(deltaTime, camera);
	m_tRenderer->render(deltaTime, camera);
}

void IMenuState::enter()
{
	if (m_isInitialized)
		return;

	m_isInitialized = true;

	// Initialize the rest of the menu's elements.
	initMenu();

	// Add close button.
	m_panel->addNewLine();
	Engine &engine{ m_engine };
	m_closeButton = std::make_shared<UIButton>("Close",
		BUTTON_SIZE, false, [&engine]()
		{
			engine.popState();
		});
	m_panel->addElement(m_closeButton);

	m_panel->addToRenderer(*m_uRenderer, *m_tRenderer);
	m_title->setAlign(TextRenderer::TextAlign::CENTER);
}

void IMenuState::addButton(std::shared_ptr<UIButton> button)
{
	m_panel->addNewLine();
	m_panel->addElement(button);
	m_buttons.push_back(button);
}

void IMenuState::getFilePath(std::string &path, const std::string &fileFormat)
{
	if (path.compare(path.length() - fileFormat.length(),
		fileFormat.length(), fileFormat))
		path += fileFormat;
}

void IMenuState::update(float deltaTime)
{
}

void IMenuState::resize(Camera &camera)
{
	m_panel->setSize(m_windowSize);

	glm::vec2 buttonPos{ (m_panel->getSize().x - BUTTON_SIZE.x) / 2.f, -1.f };
	m_title->setPosition(buttonPos);
	m_closeButton->setPosition(buttonPos);

	for (auto &button : m_buttons)
	{
		button->setPosition(buttonPos);
	}
}

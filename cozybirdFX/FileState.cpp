#include "FileState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "TextRenderer.h"
#include "UIButton.h"
#include "UIRenderer.h"
#include "UIText.h"

#include <json/single_include/nlohmann/json.hpp>
#include <nfd/nfd.h>

#include <fstream>
#include <iomanip>

namespace
{
	const glm::vec2 BUTTON_SIZE{ 500.f, 80.f };
}

FileState::FileState(Engine &engine, AssetLoader &assetLoader,
	EmitterRenderer &eRenderer)
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
		BUTTON_SIZE, false, [&engine, &assetLoader, &eRenderer]()
		{
			nfdchar_t *path{ nullptr };
			nfdresult_t result = NFD_OpenDialog("json", nullptr, &path);

			// Load if path is valid.
			if (result == NFD_OKAY)
			{
				std::ifstream input(path);
				nlohmann::json j;
				input >> j;

				eRenderer.init(assetLoader);

				eRenderer.setDuration(j["duration"].get<float>());
				eRenderer.setClipSize(glm::ivec2(j["clipSize"]["x"].get<int>(), 
					j["clipSize"]["y"].get<int>()));
				eRenderer.setExportFPS(j["exportFPS"].get<int>());
				eRenderer.setLooping(j["isLooping"].get<bool>());

				for (nlohmann::json &e : j["emitters"])
				{
					int index{ glm::clamp(e["emitter"].get<int>(), 0, 
						EmitterRenderer::NUM_EMITTERS - 1) };
					Emitter &emitter{ eRenderer.getEmitter(index) };

					eRenderer.toggleEmitter(index, true);

					emitter.setNumToGenerate(e["numToGenerate"].get<int>());
					emitter.setPosition(glm::vec2(e["position"]["x"].get<float>(), 
						e["position"]["y"].get<float>()));
					emitter.setTimeToSpawn(e["timeToSpawn"].get<float>());
					emitter.setVelocityMin(glm::vec2(e["velocityMin"]["x"].get<float>(),
						e["velocityMin"]["y"].get<float>()));
					emitter.setVelocityOffset(glm::vec2(e["velocityOffset"]["x"].get<float>(),
						e["velocityOffset"]["y"].get<float>()));
					emitter.setAcceleration(glm::vec2(e["acceleration"]["x"].get<float>(),
						e["acceleration"]["y"].get<float>()));
					emitter.setSize(e["size"].get<float>());
					emitter.setLifeMin(e["lifeMin"].get<float>());
					emitter.setLifeOffset(e["lifeOffset"].get<float>());
					emitter.setColour(glm::vec4(e["colour"]["r"].get<float>(), 
						e["colour"]["g"].get<float>(), e["colour"]["b"].get<float>(), 
						e["colour"]["a"].get<float>()));
					emitter.setAdditivity(e["additivity"].get<float>());
					emitter.setBirthColour(glm::vec4(e["birthColour"]["r"].get<float>(), 
						e["birthColour"]["g"].get<float>(), e["birthColour"]["b"].get<float>(), 
						e["birthColour"]["a"].get<float>()));
					emitter.setBirthAdditivity(e["birthAdditivity"].get<float>());
					emitter.setDeathColour(glm::vec4(e["deathColour"]["r"].get<float>(), 
						e["deathColour"]["g"].get<float>(), e["deathColour"]["b"].get<float>(), 
						e["deathColour"]["a"].get<float>()));
					emitter.setDeathAdditivity(e["deathAdditivity"].get<float>());
					emitter.setDelayBeforeStart(e["delay"].get<float>());
					emitter.setEmitterDuration(e["duration"].get<float>());
					emitter.setHSineAmplitude(e["hSineAmplitude"].get<float>());
					emitter.setHSinePeriod(e["hSinePeriod"].get<float>());
					emitter.setVSineAmplitude(e["vSineAmplitude"].get<float>());
					emitter.setVSinePeriod(e["vSinePeriod"].get<float>());
					emitter.setCircleRadius(e["circleRadius"].get<float>());
					emitter.setCirclePeriod(e["circlePeriod"].get<float>());
				}

				// Return to the previous state.
				engine.popState();
			}
			else if (result != NFD_CANCEL)
			{
				std::cout << "FileState, load path error: " <<
					NFD_GetError() << std::endl;
			}
		});
	m_panel->addElement(m_loadButton);

	m_panel->addNewLine();
	m_saveButton = std::make_shared<UIButton>("Save",
		BUTTON_SIZE, false, [&engine, &eRenderer]()
		{
			nfdchar_t *path{ nullptr };
			nfdresult_t result = NFD_SaveDialog("json", nullptr, &path);

			// Save if path is valid.
			if (result == NFD_OKAY)
			{
				nlohmann::json j;

				j["duration"] = eRenderer.getDuration();

				glm::ivec2 clipSize{ eRenderer.getClipSize() };
				j["clipSize"] = { { "x", clipSize.x }, { "y", clipSize.y } };

				j["exportFPS"] = eRenderer.getExportFPS();
				j["isLooping"] = eRenderer.isLooping();

				// Get each emitter's attributes.
				std::vector<nlohmann::json> emitters;
				for (int i = 0; i < EmitterRenderer::NUM_EMITTERS; ++i)
				{
					if (!eRenderer.isEnabled(i))
						continue;

					nlohmann::json e;
					e["emitter"] = i;

					Emitter &emitter{ eRenderer.getEmitter(i) };
					e["numToGenerate"] = emitter.getNumToGenerate();

					glm::vec2 pos{ emitter.getPosition() };
					e["position"] = { { "x", pos.x }, { "y", pos.y } };

					e["timeToSpawn"] = emitter.getTimeToSpawn();

					glm::vec2 velocityMin{ emitter.getVelocityMin() };
					e["velocityMin"] = { { "x", velocityMin.x }, { "y", velocityMin.y } };

					glm::vec2 velocityOffset{ emitter.getVelocityOffset() };
					e["velocityOffset"] = { { "x", velocityOffset.x }, { "y", velocityOffset.y } };

					glm::vec2 acceleration{ emitter.getAcceleration() };
					e["acceleration"] = { { "x", acceleration.x }, { "y", acceleration.y } };

					e["size"] = emitter.getSize();
					e["lifeMin"] = emitter.getLifeMin();
					e["lifeOffset"] = emitter.getLifeOffset();

					glm::vec4 colour{ emitter.getColour() };
					e["colour"] = { { "r", colour.r }, { "g", colour.g }, 
						{ "b", colour.b }, { "a", colour.a } };

					e["additivity"] = emitter.getAdditivity();

					glm::vec4 birthColour{ emitter.getBirthColour() };
					e["birthColour"] = { { "r", birthColour.r }, { "g", birthColour.g }, 
						{ "b", birthColour.b }, { "a", birthColour.a } };

					e["birthAdditivity"] = emitter.getBirthAdditivity();

					glm::vec4 deathColour{ emitter.getDeathColour() };
					e["deathColour"] = { { "r", deathColour.r }, { "g", deathColour.g }, 
						{ "b", deathColour.b }, { "a", deathColour.a } };

					e["deathAdditivity"] = emitter.getDeathAdditivity();
					e["delay"] = emitter.getDelayBeforeStart();
					e["duration"] = emitter.getEmitterDuration();
					e["hSineAmplitude"] = emitter.getHSineAmplitude();
					e["hSinePeriod"] = emitter.getHSinePeriod();
					e["vSineAmplitude"] = emitter.getVSineAmplitude();
					e["vSinePeriod"] = emitter.getVSinePeriod();
					e["circleRadius"] = emitter.getCircleRadius();
					e["circlePeriod"] = emitter.getCirclePeriod();

					emitters.push_back(e);
				}

				j["emitters"] = emitters;

				// Output serialized JSON.
				std::ofstream output(path);
				output << std::setw(4) << j << std::endl;
				output.close();
				free(path);

				// Return to the previous state.
				engine.popState();
			}
			else if (result != NFD_CANCEL)
			{
				std::cout << "FileState, save path error: " <<
					NFD_GetError() << std::endl;
			}
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

void FileState::enter()
{

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

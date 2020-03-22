#include "FileState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "NewState.h"
#include "UIButton.h"

#include <json/single_include/nlohmann/json.hpp>
#include <nfd/nfd.h>

#include <fstream>
#include <iomanip>

FileState::FileState(Engine &engine, AssetLoader &assetLoader,
	EmitterRenderer &eRenderer) :
	IMenuState(engine, assetLoader, eRenderer, "File Menu")
{

}

void FileState::initMenu()
{
	Engine &engine{ m_engine };
	AssetLoader &assetLoader{ m_assetLoader };
	EmitterRenderer &eRenderer{ m_eRenderer };

	m_newButton = std::make_shared<UIButton>("New",
		BUTTON_SIZE, false, [&engine, &assetLoader, &eRenderer]()
		{
			NewState *state{ new NewState(engine, assetLoader, eRenderer) };
			engine.pushState(state);
		});

	addButton(m_newButton);

	// Load button.
	m_loadButton = std::make_shared<UIButton>("Load FX...",
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

				try
				{
					eRenderer.setDuration(j.at("duration").get<float>());
					eRenderer.setClipSize(glm::ivec2(j.at("clipSize").at("x").get<int>(),
						j.at("clipSize").at("y").get<int>()));
					eRenderer.setExportFPS(j.at("exportFPS").get<int>());
					eRenderer.setLooping(j.at("isLooping").get<bool>());

					for (nlohmann::json &e : j.at("emitters"))
					{
						int index{ glm::clamp(e.at("emitter").get<int>(), 0,
							EmitterRenderer::NUM_EMITTERS - 1) };
						Emitter &emitter{ eRenderer.getEmitter(index) };

						eRenderer.toggleEmitter(index, true);

						emitter.setTexture(assetLoader, e.at("textureName").get<std::string>());
						emitter.setNumToGenerate(e.at("numToGenerate").get<int>());
						emitter.setPosition(glm::vec2(e.at("position").at("x").get<float>(),
							e.at("position").at("y").get<float>()));
						emitter.setTimeToSpawn(e.at("timeToSpawn").get<float>());
						emitter.setSpeedMin(e.at("speedMin").get<float>());
						emitter.setSpeedMax(e.at("speedMax").get<float>());
						emitter.setSpeedGrowth(e.at("speedGrowth").get<float>());
						emitter.setDirectionMin(e.at("directionMin").get<int>());
						emitter.setDirectionMax(e.at("directionMax").get<int>());
						emitter.setDirectionGrowth(e.at("directionGrowth").get<int>());
						emitter.setRotationMin(e.at("rotationMin").get<int>());
						emitter.setRotationMax(e.at("rotationMax").get<int>());
						emitter.setRotationGrowth(e.at("rotationGrowth").get<int>());
						emitter.setSizeMin(e.at("sizeMin").get<float>());
						emitter.setSizeMax(e.at("sizeMax").get<float>());
						emitter.setSizeGrowth(e.at("sizeGrowth").get<float>());
						emitter.setLifeMin(e.at("lifeMin").get<float>());
						emitter.setLifeMax(e.at("lifeMax").get<float>());
						emitter.setColour(glm::vec4(e.at("colour").at("r").get<float>(),
							e.at("colour").at("g").get<float>(), e.at("colour").at("b").get<float>(),
								e.at("colour").at("a").get<float>()));
						emitter.setAdditivity(e.at("additivity").get<float>());
						emitter.setBirthColour(glm::vec4(e.at("birthColour").at("r").get<float>(),
							e.at("birthColour").at("g").get<float>(), e.at("birthColour").at("b").get<float>(),
								e.at("birthColour").at("a").get<float>()));
						emitter.setBirthAdditivity(e.at("birthAdditivity").get<float>());
						emitter.setDeathColour(glm::vec4(e.at("deathColour").at("r").get<float>(),
							e.at("deathColour").at("g").get<float>(), e.at("deathColour").at("b").get<float>(),
								e.at("deathColour").at("a").get<float>()));
						emitter.setDeathAdditivity(e.at("deathAdditivity").get<float>());
						emitter.setDelayBeforeStart(e.at("delay").get<float>());
						emitter.setEmitterDuration(e.at("duration").get<float>());
						emitter.setHSineAmplitude(e.at("hSineAmplitude").get<float>());
						emitter.setHSinePeriod(e.at("hSinePeriod").get<float>());
						emitter.setVSineAmplitude(e.at("vSineAmplitude").get<float>());
						emitter.setVSinePeriod(e.at("vSinePeriod").get<float>());
						emitter.setCircleRadius(e.at("circleRadius").get<float>());
						emitter.setCirclePeriod(e.at("circlePeriod").get<float>());
					}
				}
				catch (nlohmann::json::out_of_range &e)
				{
					std::cout << "FileState, load: Invalid JSON format: " << e.what() << std::endl;
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

	addButton(m_loadButton);

	m_saveButton = std::make_shared<UIButton>("Save FX as...",
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

					e["textureName"] = emitter.getTextureName();
					e["numToGenerate"] = emitter.getNumToGenerate();

					glm::vec2 pos{ emitter.getPosition() };
					e["position"] = { { "x", pos.x }, { "y", pos.y } };

					e["timeToSpawn"] = emitter.getTimeToSpawn();

					glm::vec3 speed{ emitter.getSpeed() };
					e["speedMin"] = speed.x;
					e["speedMax"] = speed.y;
					e["speedGrowth"] = speed.z;

					glm::ivec3 direction{ emitter.getDirection() };
					e["directionMin"] = direction.x;
					e["directionMax"] = direction.y;
					e["directionGrowth"] = direction.z;

					glm::ivec3 rotation{ emitter.getRotation() };
					e["rotationMin"] = rotation.x;
					e["rotationMax"] = rotation.y;
					e["rotationGrowth"] = rotation.z;

					glm::vec3 size{ emitter.getSize() };
					e["sizeMin"] = size.x;
					e["sizeMax"] = size.y;
					e["sizeGrowth"] = size.z;

					glm::vec2 life{ emitter.getLife() };
					e["lifeMin"] = life.x;
					e["lifeMax"] = life.y;

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

	addButton(m_saveButton);
}
#include "NewState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "UIButton.h"

#include <glm/gtc/random.hpp>

namespace
{
	const std::vector<std::string> FIRE_TEXTURES{
		"flames", 
		"fireball1",
		"fireball2",
		"fireball3",
		"fireball4",
		"fireball5"
	};
}

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

			eRenderer.setClipSize(glm::ivec2(200, 300));
			float duration{ 3.f };
			eRenderer.setDuration(duration);

			Emitter &emitter{ eRenderer.getEmitter(0) };
			emitter.setPosition(glm::vec2(0.f, -100.f));
			emitter.setNumToGenerate(glm::linearRand(20, 40));
			emitter.setTimeToSpawn(glm::linearRand(0.4f, 0.6f));

			emitter.setSpeedMin(0.f);
			emitter.setSpeedMax(glm::linearRand(70.f, 110.f));
			emitter.setSpeedGrowth(0.f);

			int dirMin{ glm::linearRand(60, 80) };
			emitter.setDirectionMin(dirMin);
			const int DIR_CENTER{ 90 };
			emitter.setDirectionMax(DIR_CENTER * 2 - dirMin);
			emitter.setDirectionGrowth(0);

			emitter.setRotationMin(0);
			emitter.setRotationMax(359);
			emitter.setRotationGrowth(glm::linearRand(5, 30));

			float sizeMin{ glm::linearRand(24.f, 48.f) };
			emitter.setSizeMin(sizeMin);
			emitter.setSizeMax(glm::linearRand(sizeMin, sizeMin + 16.f));

			float lifeMin{ glm::linearRand(1.7f, 2.5f) };
			emitter.setLifeMin(lifeMin);
			emitter.setLifeMax(glm::linearRand(lifeMin, lifeMin + 0.2f));

			float green{ glm::linearRand(0.2f, 0.55f) };
			emitter.setColour(glm::vec4(
				glm::linearRand(0.7f, 1.f),
				green,
				glm::linearRand(0.f, green),
				glm::linearRand(0.7f, 1.f)));
			emitter.setAdditivity(1.f);
			emitter.setBirthColour(glm::vec4(
				glm::linearRand(0.6f, 0.8f),
				glm::linearRand(0.f, 0.3f),
				glm::linearRand(0.f, 0.3f),
				glm::linearRand(0.f, 0.1f)));
			emitter.setBirthAdditivity(1.f);
			emitter.setDeathColour(glm::vec4(
				glm::linearRand(0.4f, 0.6f),
				glm::linearRand(0.f, 0.3f),
				0.f, 
				glm::linearRand(0.f, 0.1f)));
			emitter.setDeathAdditivity(1.f);

			int textureIndex{ glm::linearRand(0, static_cast<int>(FIRE_TEXTURES.size()) - 1) };
			emitter.setTexture(assetLoader, FIRE_TEXTURES[textureIndex] + ".png");

			bool hasHSine{ glm::linearRand(0, 1) == 0 };
			if (hasHSine)
			{
				emitter.setHSineAmplitude(glm::linearRand(3.f, 10.f));
				emitter.setHSinePeriod(glm::linearRand(2.f, duration));
			}

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
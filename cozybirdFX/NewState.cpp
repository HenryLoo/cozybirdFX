#include "NewState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "UIButton.h"

#include <glm/gtc/random.hpp>

namespace
{
	// Generate a random float bounded by min and max inclusive.
	float getRandomFloat(float min, float max, int precision = 1)
	{
		float val{ glm::linearRand(min, max) };
		int mult{ static_cast<int>(pow(10, precision)) };
		val = glm::round(val * mult) / mult;
		return val;
	}

	const std::vector<std::string> FIRE_TEXTURES{
		"flames", 
		"fireball1",
		"fireball2",
		"fireball3",
		"fireball4",
		"fireball5"
	};

	const std::vector<std::string> SMOKE_TEXTURES{
		"smoke1",
		"smoke2",
		"smoke3",
		"cloud3",
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

			Emitter &emitter{ eRenderer.getEmitter(0) };
			emitter.setPosition(glm::vec2(0.f, -100.f));
			emitter.setNumToGenerate(glm::linearRand(20, 40));
			emitter.setTimeToSpawn(getRandomFloat(0.4f, 0.6f));

			emitter.setSpeedMin(0.f);
			emitter.setSpeedMax(getRandomFloat(70.f, 110.f));
			emitter.setSpeedGrowth(0.f);

			int dirMin{ glm::linearRand(60, 80) };
			emitter.setDirectionMin(dirMin);
			const int DIR_CENTER{ 90 };
			emitter.setDirectionMax(DIR_CENTER * 2 - dirMin);
			emitter.setDirectionGrowth(0);

			emitter.setRotationMin(0);
			emitter.setRotationMax(359);
			emitter.setRotationGrowth(glm::linearRand(5, 30));

			float sizeMin{ getRandomFloat(24.f, 48.f) };
			emitter.setSizeMin(sizeMin);
			emitter.setSizeMax(getRandomFloat(sizeMin, sizeMin + 16.f));

			float lifeMin{ getRandomFloat(1.7f, 2.5f) };
			emitter.setLifeMin(lifeMin);
			float lifeMax{ getRandomFloat(lifeMin, lifeMin + 0.2f) };
			emitter.setLifeMax(lifeMax);

			emitter.setBlendMode(Emitter::BlendMode::Additive);

			float green{ getRandomFloat(0.2f, 0.55f, 3) };
			emitter.setColour(glm::vec4(
				getRandomFloat(0.7f, 1.f, 3),
				green,
				getRandomFloat(0.f, green, 3),
				getRandomFloat(0.7f, 1.f, 3)));
			emitter.setBirthColour(glm::vec4(
				getRandomFloat(0.6f, 0.8f, 3),
				getRandomFloat(0.f, 0.3f, 3),
				getRandomFloat(0.f, 0.3f, 3),
				getRandomFloat(0.f, 0.1f, 3)));
			emitter.setDeathColour(glm::vec4(
				getRandomFloat(0.4f, 0.6f, 3),
				getRandomFloat(0.f, 0.3f, 3),
				0.f, 
				getRandomFloat(0.f, 0.1f, 3)));

			int textureIndex{ glm::linearRand(0, static_cast<int>(FIRE_TEXTURES.size()) - 1) };
			emitter.setTexture(assetLoader, FIRE_TEXTURES[textureIndex] + ".png");

			bool hasHSine{ glm::linearRand(0, 1) == 0 };
			if (hasHSine)
			{
				emitter.setHSineAmplitude(getRandomFloat(3.f, 10.f));
				emitter.setHSinePeriod(getRandomFloat(2.f, lifeMax));
			}

			engine.popState();
			engine.popState();
		});

	addButton(m_fireButton);

	m_smokeButton = std::make_shared<UIButton>("Smoke",
		BUTTON_SIZE, false, [&engine, &assetLoader, &eRenderer]()
		{
			eRenderer.init(assetLoader);

			eRenderer.setClipSize(glm::ivec2(250, 320));

			Emitter &emitter{ eRenderer.getEmitter(0) };
			emitter.setPosition(glm::vec2(0.f, -90.f));
			emitter.setNumToGenerate(glm::linearRand(4, 7));
			emitter.setTimeToSpawn(getRandomFloat(0.6f, 0.9f));

			emitter.setSpeedMin(0.f);
			emitter.setSpeedMax(getRandomFloat(60.f, 90.f));
			emitter.setSpeedGrowth(0.f);

			int dirMin{ glm::linearRand(60, 80) };
			emitter.setDirectionMin(dirMin);
			const int DIR_CENTER{ 90 };
			emitter.setDirectionMax(DIR_CENTER * 2 - dirMin);
			emitter.setDirectionGrowth(0);

			emitter.setRotationMin(0);
			emitter.setRotationMax(359);
			emitter.setRotationGrowth(glm::linearRand(5, 30));

			float sizeMin{ getRandomFloat(70.f, 90.f) };
			emitter.setSizeMin(sizeMin);
			emitter.setSizeMax(getRandomFloat(sizeMin, sizeMin + 20.f));

			float lifeMin{ getRandomFloat(2.5f, 2.8f) };
			emitter.setLifeMin(lifeMin);
			float lifeMax{ getRandomFloat(lifeMin, lifeMin + 0.2f) };
			emitter.setLifeMax(lifeMax);

			emitter.setBlendMode(Emitter::BlendMode::Linear);

			float col{ getRandomFloat(0.45f, 0.55f, 3) };
			float colOffset{ 0.1f };
			emitter.setColour(glm::vec4(
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(0.7f, 1.f, 3)));
			emitter.setBirthColour(glm::vec4(
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(col - colOffset, col + colOffset, 3),
				0.f));
			emitter.setDeathColour(glm::vec4(
				getRandomFloat(col - colOffset / 2, col + colOffset / 2, 3),
				getRandomFloat(col - colOffset / 2, col + colOffset / 2, 3),
				getRandomFloat(col - colOffset / 2, col + colOffset / 2, 3),
				0.f));

			int textureIndex{ glm::linearRand(0, static_cast<int>(SMOKE_TEXTURES.size()) - 1) };
			emitter.setTexture(assetLoader, SMOKE_TEXTURES[textureIndex] + ".png");

			engine.popState();
			engine.popState();
		});

	addButton(m_smokeButton);
}
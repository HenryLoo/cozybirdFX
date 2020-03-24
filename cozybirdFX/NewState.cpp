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

	const std::vector<std::string> BLAST_TEXTURES{
		"blast1",
		"blast2",
		"blast3",
		"blast4",
		"blast5",
		"blast6",
		"blast7",
		"blast8",
		"blast9",
		"blast10",
	};

	const std::vector<std::string> SPARK_TEXTURES{
		"sparkle_flare1",
		"sparkle_flare2",
		"sparkle_flare3",
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

			emitter.setBirthColour(glm::vec4(
				getRandomFloat(0.6f, 0.8f, 3),
				getRandomFloat(0.f, 0.3f, 3),
				getRandomFloat(0.f, 0.3f, 3),
				getRandomFloat(0.f, 0.1f, 3)));
			float green{ getRandomFloat(0.2f, 0.55f, 3) };
			emitter.setColour(glm::vec4(
				getRandomFloat(0.7f, 1.f, 3),
				green,
				getRandomFloat(0.f, green, 3),
				getRandomFloat(0.7f, 1.f, 3)));
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
			emitter.setBirthColour(glm::vec4(
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(col - colOffset, col + colOffset, 3),
				0.f));
			emitter.setColour(glm::vec4(
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(col - colOffset, col + colOffset, 3),
				getRandomFloat(0.7f, 1.f, 3)));
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

	m_explosionButton = std::make_shared<UIButton>("Explosion",
		BUTTON_SIZE, false, [&engine, &assetLoader, &eRenderer]()
		{
			eRenderer.init(assetLoader);

			eRenderer.setClipSize(glm::ivec2(320, 320));
			eRenderer.setLooping(false);

			// Blast ring.
			Emitter &emitter0{ eRenderer.getEmitter(0) };
			emitter0.setNumToGenerate(1);
			emitter0.setTimeToSpawn(2.5f);

			emitter0.setSpeedMin(0.f);
			emitter0.setSpeedMax(0.f);
			emitter0.setSpeedGrowth(0.f);

			emitter0.setDirectionMin(0);
			emitter0.setDirectionMax(359);
			emitter0.setDirectionGrowth(0);

			emitter0.setRotationMin(0);
			emitter0.setRotationMax(0);
			emitter0.setRotationGrowth(0);

			emitter0.setSizeMin(0.f);
			emitter0.setSizeMax(0.f);
			emitter0.setSizeGrowth(200.f);

			float life{ getRandomFloat(0.4f, 0.6f) };
			emitter0.setLifeMin(life);
			emitter0.setLifeMax(life);

			emitter0.setBlendMode(Emitter::BlendMode::Additive);

			emitter0.setBirthColour(glm::vec4(
				1.f,
				getRandomFloat(0.6f, 0.9f, 3),
				getRandomFloat(0.f, 0.3f, 3),
				1.f));
			emitter0.setColour(glm::vec4(
				1.f,
				getRandomFloat(0.3f, 0.6f, 3),
				0.f,
				1.f));
			emitter0.setDeathColour(glm::vec4(
				1.f,
				getRandomFloat(0.f, 0.3f, 3),
				0.f,
				1.f));

			int textureIndex{ glm::linearRand(0, static_cast<int>(BLAST_TEXTURES.size()) - 1) };
			emitter0.setTexture(assetLoader, BLAST_TEXTURES[textureIndex] + ".png");

			// Flash.
			Emitter &emitter1{ eRenderer.getEmitter(1) };
			emitter1.setNumToGenerate(1);
			emitter1.setTimeToSpawn(2.5f);

			emitter1.setSpeedMin(0.f);
			emitter1.setSpeedMax(0.f);
			emitter1.setSpeedGrowth(0.f);

			emitter1.setDirectionMin(0);
			emitter1.setDirectionMax(359);
			emitter1.setDirectionGrowth(0);

			emitter1.setRotationMin(0);
			emitter1.setRotationMax(0);
			emitter1.setRotationGrowth(0);

			emitter1.setSizeMin(100.f);
			emitter1.setSizeMax(100.f);
			emitter1.setSizeGrowth(-200.f);

			life = getRandomFloat(0.1f, 0.3f);
			emitter1.setLifeMin(life);
			emitter1.setLifeMax(life);

			emitter1.setBlendMode(Emitter::BlendMode::Linear);

			emitter1.setBirthColour(glm::vec4(
				1.f,
				1.f,
				1.f,
				1.f));
			emitter1.setColour(glm::vec4(
				1.f,
				getRandomFloat(0.8f, 0.9f, 3),
				getRandomFloat(0.7f, 0.8f, 3),
				1.f));
			emitter1.setDeathColour(glm::vec4(
				1.f,
				getRandomFloat(0.3f, 0.5f, 3),
				0.f,
				0.f));

			emitter1.setTexture(assetLoader, "circle.png");

			// Fire ring.
			Emitter &emitter2{ eRenderer.getEmitter(2) };
			emitter2.setEmitterDuration(getRandomFloat(0.3f, 0.5f));
			emitter2.setNumToGenerate(300);
			emitter2.setTimeToSpawn(1.f);

			float speed{ getRandomFloat(80.f, 100.f) };
			emitter2.setSpeedMin(speed);
			emitter2.setSpeedMax(speed);
			emitter2.setSpeedGrowth(0.f);

			emitter2.setDirectionMin(0);
			emitter2.setDirectionMax(359);
			emitter2.setDirectionGrowth(0);

			emitter2.setRotationMin(0);
			emitter2.setRotationMax(359);
			emitter2.setRotationGrowth(glm::linearRand(5, 30));

			float size{ getRandomFloat(18.f, 23.f) };
			emitter2.setSizeMin(size);
			emitter2.setSizeMax(getRandomFloat(size, size + 12.f));
			emitter2.setSizeGrowth(getRandomFloat(-10.f, -5.f));

			life = getRandomFloat(1.f, 1.3f);
			emitter2.setLifeMin(life);
			emitter2.setLifeMax(getRandomFloat(life, life + 0.2f));

			emitter2.setBlendMode(Emitter::BlendMode::Additive);

			emitter2.setBirthColour(glm::vec4(
				1.f,
				1.f,
				1.f,
				0.f));
			emitter2.setColour(glm::vec4(
				1.f,
				getRandomFloat(0.3f, 0.5f, 3),
				0.f,
				1.f));
			emitter2.setDeathColour(glm::vec4(
				1.f,
				getRandomFloat(0.1f, 0.3f, 3),
				0.f,
				0.f));

			textureIndex = glm::linearRand(0, static_cast<int>(FIRE_TEXTURES.size()) - 1);
			emitter2.setTexture(assetLoader, FIRE_TEXTURES[textureIndex] + ".png");

			// Burn flames.
			Emitter &emitter3{ eRenderer.getEmitter(3) };
			emitter3.setEmitterDuration(getRandomFloat(0.3f, 0.5f));
			emitter3.setNumToGenerate(glm::linearRand(6, 10));
			emitter3.setTimeToSpawn(1.f);

			speed = getRandomFloat(12.f, 25.f);
			emitter3.setSpeedMin(speed);
			emitter3.setSpeedMax(getRandomFloat(speed, speed + 10.f));
			emitter3.setSpeedGrowth(getRandomFloat(-20.f, -10.f));

			emitter3.setDirectionMin(0);
			emitter3.setDirectionMax(359);
			emitter3.setDirectionGrowth(0);

			emitter3.setRotationMin(0);
			emitter3.setRotationMax(359);
			emitter3.setRotationGrowth(glm::linearRand(5, 10));

			size = getRandomFloat(50.f, 70.f);
			emitter3.setSizeMin(size);
			emitter3.setSizeMax(getRandomFloat(size, size + 40.f));
			emitter3.setSizeGrowth(0.f);

			life = getRandomFloat(0.5f, 0.7f);
			emitter3.setLifeMin(life);
			emitter3.setLifeMax(getRandomFloat(life, life + 0.2f));

			emitter3.setBlendMode(Emitter::BlendMode::Additive);

			emitter3.setBirthColour(glm::vec4(
				1.f,
				getRandomFloat(0.4f, 0.6f, 3),
				1.f,
				0.f));
			emitter3.setColour(glm::vec4(
				1.f,
				getRandomFloat(0.f, 0.2f, 3),
				getRandomFloat(0.f, 0.2f, 3),
				1.f));
			emitter3.setDeathColour(glm::vec4(
				getRandomFloat(0.3f, 0.5f, 3),
				getRandomFloat(0.f, 0.2f, 3),
				0.f,
				0.f));

			textureIndex = glm::linearRand(0, static_cast<int>(FIRE_TEXTURES.size()) - 1);
			emitter3.setTexture(assetLoader, FIRE_TEXTURES[textureIndex] + ".png");

			// Sparks.
			Emitter &emitter4{ eRenderer.getEmitter(4) };
			emitter4.setEmitterDuration(getRandomFloat(0.3f, 0.5f));
			emitter4.setNumToGenerate(glm::linearRand(30, 50));
			emitter4.setTimeToSpawn(1.f);

			speed = getRandomFloat(150.f, 180.f);
			emitter4.setSpeedMin(speed);
			emitter4.setSpeedMax(getRandomFloat(speed, speed + 30.f));
			emitter4.setSpeedGrowth(getRandomFloat(-10.f, -5.f));

			emitter4.setDirectionMin(0);
			emitter4.setDirectionMax(359);
			emitter4.setDirectionGrowth(0);

			emitter4.setRotationMin(0);
			emitter4.setRotationMax(359);
			emitter4.setRotationGrowth(0);

			size = getRandomFloat(40.f, 60.f);
			emitter4.setSizeMin(size);
			emitter4.setSizeMax(size);
			emitter4.setSizeGrowth(getRandomFloat(-20.f, -10.f));

			life = getRandomFloat(0.5f, 0.7f);
			emitter4.setLifeMin(life);
			emitter4.setLifeMax(getRandomFloat(life, life + 0.2f));

			emitter4.setBlendMode(Emitter::BlendMode::Additive);

			emitter4.setBirthColour(glm::vec4(
				1.f,
				1.f,
				1.f,
				0.f));
			emitter4.setColour(glm::vec4(
				1.f,
				getRandomFloat(0.4f, 0.6f, 3),
				0.f,
				1.f));
			emitter4.setDeathColour(glm::vec4(
				1.f,
				getRandomFloat(0.f, 0.3f, 3),
				0.f,
				getRandomFloat(0.1f, 0.2f, 3)));

			textureIndex = glm::linearRand(0, static_cast<int>(SPARK_TEXTURES.size()) - 1);
			emitter4.setTexture(assetLoader, SPARK_TEXTURES[textureIndex] + ".png");

			// Enable all 5 emitters.
			for (int i = 0; i < 5; ++i)
				eRenderer.toggleEmitter(i, true);

			engine.popState();
			engine.popState();
		});

	addButton(m_explosionButton);
}
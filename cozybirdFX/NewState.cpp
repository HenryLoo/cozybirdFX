#include "NewState.h"
#include "EmitterRenderer.h"
#include "Engine.h"
#include "UIButton.h"

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
			// TODO;
			eRenderer.init(assetLoader);

			eRenderer.setClipSize(glm::ivec2(200, 280));
			eRenderer.setDuration(3.f);

			Emitter &emitter{ eRenderer.getEmitter(0) };
			emitter.setPosition(glm::vec2(0.f, -100.f));
			emitter.setPosition(glm::vec2(0.f, -100.f));
			emitter.setNumToGenerate(30);
			emitter.setTimeToSpawn(0.5f);
			emitter.setSize(32.f);
			emitter.setLifeMin(2.f);
			emitter.setLifeOffset(0.3f);
			emitter.setVelocityMin(glm::vec2(-32.f, 0.f));
			emitter.setVelocityOffset(glm::vec2(64.f, 100.f));

			emitter.setColour(glm::vec4(1.f, 0.43f, 0.23f, 1.f));
			emitter.setAdditivity(1.f);
			emitter.setBirthColour(glm::vec4(0.76f, 0.2f, 0.2f, 0.f));
			emitter.setBirthAdditivity(0.f);
			emitter.setDeathColour(glm::vec4(0.51f, 0.29f, 0.f, 0.05f));
			emitter.setDeathAdditivity(0.f);

			emitter.setTexture(assetLoader, "particle.png");

			emitter.setHSineAmplitude(5.f);
			emitter.setHSinePeriod(3.f);

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
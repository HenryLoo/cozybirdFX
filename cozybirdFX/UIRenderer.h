#pragma once

#include "IRenderer.h"

#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>

class AssetLoader;
class Shader;
class IUIInterface;

class UIRenderer : public IRenderer
{
public:
	struct Properties
	{
		// Top-left position.
		glm::vec2 pos{ 0.f };

		// Width and height.
		glm::vec2 size{ 0.f };

		glm::vec4 colour{ 1.f };
		bool hasBorder{ false };
	};

	UIRenderer(AssetLoader *assetLoader);
	~UIRenderer();

	virtual void update(float deltaTime);

	virtual void render(Camera *camera);

	// Add a UI element to the list to be rendered.
	std::list<Properties>::iterator addElement(const Properties &prop);

	// Remove all text from the list.
	void clearElements();

private:
	// The renderer's shader program.
	std::shared_ptr<Shader> m_shader;

	// Vertex array object.
	unsigned int m_VAO;

	// Vertex buffer objects.
	unsigned int m_verticesVBO, m_borderVBO, m_colourVBO, m_modelVBO;

	// Element buffer object.
	unsigned int m_verticesEBO;

	// UI elements to render.
	std::list<Properties> m_elements;
};
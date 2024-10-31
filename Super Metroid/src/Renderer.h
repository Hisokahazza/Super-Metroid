#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include "Resources.h"

#include "imgui-SFML.h"
#include "imgui.h"

enum Direction
{
	RIGHT,
	LEFT,
	UPRIGHT,
	UPLEFT,
	DOWNRIGHT,
	DOWNLEFT,
	UP,
	DOWN
};

class Renderer
{
private:
	sf::Sprite m_Sprite{};

public:
	Renderer(sf::RenderTarget& target);
	~Renderer();

	void loadTex();
	void draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size, float angle = 0.0f, sf::IntRect textureRect = sf::IntRect());
	void drawRectangleShape(sf::RectangleShape& rectangle, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& fillColour);
	void drawText(sf::Text& text, const sf::Vector2f& position, const sf::Vector2f& size, std::string& textContents);

	sf::RenderTarget& target;
};
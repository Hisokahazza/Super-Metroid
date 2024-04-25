#pragma once

#include<vector>
#include<SFML/Graphics.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>

#include "Renderer.h"
#include "Resources.h"
#include "Physics.h"

class Map
{
private:
	std::vector<std::vector<int>> grid;
	float m_CellSize;
public:
	Map(float cellSize = 32.0f);
	~Map();

	sf::Vector2f samusPosition;

	void setBackground(Renderer& renderer, const std::string& backgroundFilePath, const sf::Vector2f& backgroundSize);
	sf::Vector2f createFromImg(const sf::Image& image);
	void draw(Renderer& renderer);
};


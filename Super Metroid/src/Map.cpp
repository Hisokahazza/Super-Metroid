#include "Map.h"
#include <iostream>

Map::Map(float cellSize) : m_CellSize(cellSize), grid()
{
}

Map::~Map()
{
}

void Map::setBackground(Renderer& renderer, const std::string& backgroundFilePath, const sf::Vector2f& backgroundSize)
{
	renderer.draw(Resources::textures[backgroundFilePath], sf::Vector2f(1920 / 2, 1080 / 2), backgroundSize);
}

sf::Vector2f Map::createFromImg(const sf::Image& image)
{
	grid.clear();
	grid = std::vector(image.getSize().x, std::vector(image.getSize().y, 0));

	for (size_t x = 0; x < grid.size(); x++)
	{
		for (size_t y = 0; y < grid[x].size(); y++)
		{
			sf::Color colour = image.getPixel(x, y);
			if (colour == sf::Color::Black)
			{
				grid[x][y] = 1;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize / 2, m_CellSize / 2);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == sf::Color::Red)
			{
				samusPosition = sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
			}
		}
	}
	return samusPosition;
}

void Map::draw(Renderer& renderer)
	{
		int x = 0;
		for (const auto& column : grid)
		{
			int y = 0;
			for (const auto& cell : column)
			{
				if (cell)
				{
					if (grid[x][y] == 1)
					{
						renderer.draw(Resources::textures["Eg_tile.png"],
							sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
								m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
					}
				}
				y++;
			}
			x++;
		}
	}

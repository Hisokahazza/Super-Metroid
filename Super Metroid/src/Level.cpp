#include "Level.h"
#include <iostream>

// Clear all bodies in a level
void Level::clearLevel()
{
	for (auto& body : m_BodiesToDelete)
	{
		/*if (m_BodiesToDelete.size() > 0)
		{
			m_BodiesToDelete.erase(std::find(m_BodiesToDelete.begin(), m_BodiesToDelete.end(), body));
		}*/

		if (body)
		{
			m_CurrentFixtureToDelete = body->GetFixtureList();

			body->DestroyFixture(m_CurrentFixtureToDelete);

			Physics::world.DestroyBody(body);

			body = nullptr;
		}
	}
	for (auto door : m_InteractableDoors)
	{

		m_InteractableDoors.erase(std::find(m_InteractableDoors.begin(), m_InteractableDoors.end(), door));

		door->destroyFixture();
		delete door;
	}
}

StageHub::StageHub(float cellSize) : m_CellSize(cellSize)
{
}

// Generates a grid with integr values to denote tiles based on an image comprised of colours
std::vector<sf::Vector2f> StageHub::createFromImg(const sf::Image& image)
{
	m_Grid.clear();
	// Create a grid of the same size as the image
	m_Grid = std::vector(image.getSize().x, std::vector(image.getSize().y, 0));

	// Check each pixel in the image and assign a value to a location in the grid dependent on the colour
	// Also assigns fixtures where relevant
	for (size_t x = 0; x < m_Grid.size(); x++)
	{
		for (size_t y = 0; y < m_Grid[x].size(); y++)
		{
			sf::Color colour = image.getPixel(x, y);

			if (colour == colours[DARKGREEN])
			{
				m_Grid[x][y] = 1;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

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
			else if (colour == colours[GREEN])
			{
				m_Grid[x][y] = 2;
			}
			else if (colour == colours[ORANGE])
			{
				m_Grid[x][y] = 2;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

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
			else if (colour == colours[BLUE])
			{
				m_Grid[x][y] = 3;
			}
			else if (colour == colours[YELLOW])
			{
				m_Grid[x][y] = 4;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

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
			else if (colour == colours[LIGHTBLUE])
			{
				m_Grid[x][y] = 5;
			}
			else if (colour == colours[BROWN])
			{
				m_Grid[x][y] = 6;
			}
			else if (colour == colours[PINK])
			{
				m_Grid[x][y] = 7;
			}
			else if (colour == colours[TURQUOISE])
			{
				m_Grid[x][y] = 8;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape polygonShape{};
				polygonShape.SetAsBox(m_CellSize * 4.0f, m_CellSize * 1.5f);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &polygonShape;
				fixtureDef.density = 0.0f;
				body->CreateFixture(&fixtureDef);

				b2CircleShape circleShape;
				circleShape.m_radius = m_CellSize * 2;
				circleShape.m_p.Set(-3.0f, 0.0f);

				fixtureDef.shape = &circleShape;
				body->CreateFixture(&fixtureDef);

				circleShape.m_p.Set(3.0f, 0.0f);
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[ROUGE])
			{
				m_Grid[x][y] = 5;

				if (x < 3)
				{
					m_Door = new Door(NOMENU, RIGHT);
				}
				else
				{
					m_Door = new Door(BOSSMENU, LEFT);
				}
				
				m_Door->setPosition(sf::Vector2f(m_CellSize* x + m_CellSize / 2.0f,
					m_CellSize* y + m_CellSize / 2.0f));
				m_Door->Begin();

				m_InteractableDoors.push_back(m_Door);
			}
			else if (colour == colours[RED])
			{
				samusPosition = sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
			}
		}
	}
	return {samusPosition, sf::Vector2f()};
}

// Goes through each value in the grid vector and renders tiles to those locations within the vector
void StageHub::draw(Renderer& renderer)
{
	int x = 0;
	for (const auto& column : m_Grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				if (m_Grid[x][y] == 4)
				{
				}
				else if (m_Grid[x][y] == 5)
				{
					renderer.draw(Resources::textures["HUB_Background_Tile_01.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 6)
				{
					renderer.draw(Resources::textures["HUB_Mountains.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize * 8.0f, m_CellSize * 3.0f));
				}
			}
			y++;
		}
		x++;
	}

	// Parse through grid a second time to render tiles on the top of the order
	x = 0;
	for (const auto& column : m_Grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				if (m_Grid[x][y] == 1)
				{
					renderer.draw(Resources::textures["HUB_Floor_Tile_02.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 3)
				{
					renderer.draw(Resources::textures["HUB_Floor_Tile_03.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize * 3, m_CellSize));
				}
				else if (m_Grid[x][y] == 7)
				{
					renderer.draw(Resources::textures["HUB_Background_Tile_02.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize * 7, m_CellSize * 2));
				}
			}
			y++;
		}
		x++;
	}

	// Parse through grid a third time to render ship and walls/floor 
	x = 0;
	for (const auto& column : m_Grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				if (m_Grid[x][y] == 8)
				{
					renderer.draw(Resources::textures["HUB_Starship.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize * 12.0f, m_CellSize * 5.5f));
				}
				else if (m_Grid[x][y] == 2)
				{
					renderer.draw(Resources::textures["HUB_Floor_Tile_01.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
			}
			y++;
		}
		x++;
	}
}

StageSporeSpawn::StageSporeSpawn(float cellSize) : m_CellSize(cellSize)
{
}

std::vector<sf::Vector2f> StageSporeSpawn::createFromImg(const sf::Image& image)
{
	m_Grid.clear();
	m_Grid = std::vector(image.getSize().x, std::vector(image.getSize().y, 0));

	for (size_t x = 0; x < m_Grid.size(); x++)
	{
		for (size_t y = 0; y < m_Grid[x].size(); y++)
		{
			sf::Color colour = image.getPixel(x, y);
			if (colour == colours[YELLOW])
			{
				m_Grid[x][y] = 1;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize / 2.0f, m_CellSize / 2.0f);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[GREEN])
			{
				m_Grid[x][y] = 2;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

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
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[DARKGREEN])
			{
				m_Grid[x][y] = 3;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

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
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[MAGENTA])
			{
				m_Grid[x][y] = 4;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + (m_CellSize / 2.0f) - 0.5f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize * 1.5, m_CellSize);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[LIGHTBLUE])
			{
				m_Grid[x][y] = 5;
			}
			else if (colour == colours[DARKBLUE])
			{
				m_Grid[x][y] = 6;
			}
			else if (colour == colours[ORANGE])
			{
				m_Grid[x][y] = 7;
			}
			else if (colour == colours[PURPLE])
			{
				m_Grid[x][y] = 8;
			}
			else if (colour == colours[BROWN])
			{
				m_Grid[x][y] = 9;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize * 7, m_CellSize / 2);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[BLACK])
			{
				m_Grid[x][y] = 10;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize* x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize / 2.0f, m_CellSize / 2.0f);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[PINK])
			{
				m_Grid[x][y] = 11;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize* x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize / 2.0f, m_CellSize / 2.0f);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[TURQUOISE])
			{
				m_Grid[x][y] = 12;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize / 2.0f, m_CellSize / 2.0f);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
				}
			else if (colour == colours[RED])
			{
				m_Grid[x][y] = 5;

				samusPosition = sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
			}
			else if (colour == colours[BLUE])
			{
				m_Grid[x][y] = 5;

				bossPosition = sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
			}
		}
	}
	return {samusPosition, bossPosition};
}

void StageSporeSpawn::draw(Renderer& renderer)
{
	int x = 0;
	for (const auto& column : m_Grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				if (m_Grid[x][y] == 1)
				{
					renderer.draw(Resources::textures["SSP_Tile_01.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 2)
				{
					renderer.draw(Resources::textures["SSP_Tile_02.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 3)
				{
					renderer.draw(Resources::textures["SSP_Tile_03.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 4)
				{
					renderer.draw(Resources::textures["SSP_Green_Door.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + (m_CellSize / 2) - 0.5f), sf::Vector2f(m_CellSize * 3, m_CellSize * 2));
				}
				else if(m_Grid[x][y] == 5)
				{
					renderer.draw(Resources::textures["SSP_Background_Tile_01.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + (m_CellSize / 2)), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 6)
				{
					renderer.draw(Resources::textures["SSP_Background_Tile_02L.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + (m_CellSize / 2)), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 7)
				{
					renderer.draw(Resources::textures["SSP_Background_Tile_02R.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + (m_CellSize / 2)), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 8)
				{
					renderer.draw(Resources::textures["SSP_Background_Tile_03.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + (m_CellSize / 2)), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 9)
				{
					renderer.draw(Resources::textures["SSP_Ceiling.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + (m_CellSize / 2)), sf::Vector2f(m_CellSize * 15, m_CellSize * 1.5));
				}
				else if (m_Grid[x][y] == 10)
				{
					renderer.draw(Resources::textures["SSP_Spores.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + (m_CellSize / 2)), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 11)
				{
					renderer.draw(Resources::textures["SSP_Tile_04L.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + (m_CellSize / 2)), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 12)
				{
					renderer.draw(Resources::textures["SSP_Tile_04R.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + (m_CellSize / 2)), sf::Vector2f(m_CellSize, m_CellSize));
				}
			}
			y++;
		}
		x++;
	}
}

StageGoldTorizo::StageGoldTorizo(float cellSize) : m_CellSize(cellSize)
{
}

std::vector<sf::Vector2f> StageGoldTorizo::createFromImg(const sf::Image& image)
{
	m_Grid.clear();
	m_Grid = std::vector(image.getSize().x, std::vector(image.getSize().y, 0));

	for (size_t x = 0; x < m_Grid.size(); x++)
	{
		for (size_t y = 0; y < m_Grid[x].size(); y++)
		{
			sf::Color colour = image.getPixel(x, y);
			if (colour == colours[GREEN])
			{
				m_Grid[x][y] = 1;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize / 2.0f, m_CellSize / 2.0f);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[PURPLE])
			{
				m_Grid[x][y] = 2;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize, m_CellSize / 2.0f);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[TURQUOISE])
			{
				m_Grid[x][y] = 3;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize / 2.0f, m_CellSize / 2.0f);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[MAGENTA])
			{
				m_Grid[x][y] = 4;

				b2BodyDef bodyDef{};
				bodyDef.position.Set(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				m_BodiesToDelete.push_back(body);

				b2PolygonShape shape{};
				shape.SetAsBox(m_CellSize, m_CellSize * 1.5);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = MAPTILE;
				fixtureData->mapPosx = x;
				fixtureData->mapPosY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.shape = &shape;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				body->CreateFixture(&fixtureDef);
			}
			else if (colour == colours[ORANGE])
			{
				m_Grid[x][y] = 5;
			}
			else if (colour == colours[BLUE])
			{
				m_Grid[x][y] = 6;
			}
			else if (colour == colours[YELLOW])
			{
				m_Grid[x][y] = 7;
			}
			else if (colour == colours[ROUGE])
			{
				m_Grid[x][y] = 8;
			}
			else if (colour == colours[DARKGREEN])
			{
				m_Grid[x][y] = 9;
			}
			else if (colour == colours[PINK])
			{
				m_Grid[x][y] = 10;
			}
			else if (colour == colours[LIGHTBLUE])
			{
				m_Grid[x][y] = 11;
			}
			else if (colour == colours[RED])
			{
				//m_Grid[x][y] = 6;

				samusPosition = sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
			}
			else if (colour == colours[BROWN])
			{
				//m_Grid[x][y] = 6;

				bossPosition = sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
					m_CellSize * y + m_CellSize / 2.0f);
			}
		}
	}
	
	return { samusPosition, bossPosition };
}

void StageGoldTorizo::draw(Renderer& renderer)
{
	int x = 0;
	for (const auto& column : m_Grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				if (m_Grid[x][y] == 1)
				{
					renderer.draw(Resources::textures["GT_Background_Tile_01.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 3)
				{
					renderer.draw(Resources::textures["GT_Background_Tile_01.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 4)
				{
					renderer.draw(Resources::textures["GT_Blue_Door.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize * 2, m_CellSize * 3));
				}
				else if (m_Grid[x][y] == 5)
				{
					renderer.draw(Resources::textures["GT_Background_Tile_03.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 6)
				{

					renderer.draw(Resources::textures["GT_Background_Tile_02.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 7)
				{
					renderer.draw(Resources::textures["GT_Background_Tile_04.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 8)
				{
					renderer.draw(Resources::textures["GT_Background_Tile_05.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 9)
				{
					renderer.draw(Resources::textures["GT_Background_Tile_06.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 10)
				{
					renderer.draw(Resources::textures["GT_Background_Tile_07.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize, m_CellSize));
				}
				else if (m_Grid[x][y] == 11)
				{
					renderer.draw(Resources::textures["GT_Stalactites.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize * 3, m_CellSize * 3));
				}
			}
			y++;
		}
		x++;
	}

	x = 0;
	for (const auto& column : m_Grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				if (m_Grid[x][y] == 2)
				{
					renderer.draw(Resources::textures["GT_Pillar_01.png"],
						sf::Vector2f(m_CellSize * x + m_CellSize / 2.0f,
							m_CellSize * y + m_CellSize / 2.0f), sf::Vector2f(m_CellSize * 2, m_CellSize));
				}
			}
			y++;
		}
		x++;
	}
}

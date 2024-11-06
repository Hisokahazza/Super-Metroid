#pragma once

#include <vector>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

#include "Renderer.h"
#include "Resources.h"
#include "Physics.h"
#include "Door.h"

enum Colour
{
	RED,
	GREEN,
	BLUE,
	DARKBLUE,
	LIGHTBLUE,
	YELLOW,
	DARKGREEN,
	BROWN,
	BLACK,
	WHITE,
	MAGENTA,
	ORANGE,
	PURPLE,
	PINK,
	TURQUOISE,
	ROUGE
};

class Level
{
private:

	int m_Count = 0;
protected:
	std::vector<std::vector<int>> m_Grid;
	std::vector<b2Body*> m_BodiesToDelete;
	b2Fixture* m_CurrentFixtureToDelete;
	b2Fixture* m_NextFixtureToDelete;

	std::unordered_map<Colour, sf::Color> colours = { 
		{RED, sf::Color(255, 0, 0)},
		{GREEN, sf::Color(0, 255, 0)},
		{BLUE, sf::Color(0, 0, 255)},
		{DARKBLUE, sf::Color(10, 0, 50)},
		{LIGHTBLUE, sf::Color(0, 255, 255)},
		{YELLOW, sf::Color(255, 232, 0)},
		{DARKGREEN, sf::Color(10, 71, 9)},
		{BROWN, sf::Color(115, 29, 29)},
		{BLACK, sf::Color(0, 0, 0)},
		{WHITE, sf::Color(255, 255, 255)},
		{MAGENTA, sf::Color(255, 0, 241)},
		{ORANGE, sf::Color(255, 99, 0)},
		{PURPLE, sf::Color(120, 0, 255)},
		{PINK, sf::Color(255, 0, 255)},
		{TURQUOISE, sf::Color(0, 255, 130)},
		{ROUGE, sf::Color(171, 18, 57)}
	};

	sf::Vector2f samusPosition;
	sf::Vector2f bossPosition;

	std::vector<Door*> m_InteractableDoors;
	Door* m_Door;

public:
	void clearLevel();

	virtual std::vector<sf::Vector2f> createFromImg(const sf::Image& image) = 0;
	virtual void draw(Renderer& renderer) = 0;
	
	// Getters and setters
	std::vector<Door*> getDoors() { return m_InteractableDoors; };
};

class StageHub : public Level
{
private:
	float m_CellSize;
public:
	StageHub(float cellSize = 1.0f);

	std::vector<sf::Vector2f> createFromImg(const sf::Image& image) override;
	void draw(Renderer& renderer) override;
};

class StageSporeSpawn : public Level
{
private:
	float m_CellSize;
public:
	StageSporeSpawn(float cellSize = 1.0f);

	std::vector<sf::Vector2f> createFromImg(const sf::Image& image) override;
	void draw(Renderer& renderer) override;
};

class StageGoldTorizo : public Level
{
private:
	float m_CellSize;
public:
	StageGoldTorizo(float cellSize = 1.0f);

	std::vector<sf::Vector2f> createFromImg(const sf::Image& image) override;
	void draw(Renderer& renderer) override;
};


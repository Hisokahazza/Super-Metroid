#include "Game.h"

// Declaring objects
Camera camera(20.0f);
Samus samus;
HUD playerHUD;

SporeSpawn sporeSpawn;

float movementSpeed = 0.5f;

void Game::createStages()
{
	m_Stages["res/Level_Hub.png"] = new StageHub();
	m_Stages["res/Level_Spore_Spawn.png"] = new StageSporeSpawn();
}

Game::Game()
{
}

Game::~Game()
{
}

// Begin function
void Game::Begin(const sf::Window& window)
{
	createStages();

	Physics::init();

	//load map image
	sf::Image image;
	image.loadFromFile(m_CurrentStage);

	camera.position = sf::Vector2f(image.getSize().x / 2.0f, image.getSize().y / 2.0f);
	camera.scaleView = sf::Vector2f(1.0f, 1.0f);

	// render map and return initial samus and boss position
	std::vector<sf::Vector2f> mapPositions;
	mapPositions = m_Stages[m_CurrentStage]->createFromImg(image);
	
	samus.position = mapPositions[0];
	samus.begin();

	playerHUD.begin();

	sporeSpawn.position = mapPositions[1];
	sporeSpawn.begin();
}

// update function(called every frame)
void Game::update(float deltaTime)
{
	Physics::update(deltaTime);
	samus.update(deltaTime);

	sporeSpawn.update(deltaTime);
}

// Final rendering step
void Game::draw(Renderer& renderer)
{
	//sporeSpawnStage.draw(renderer);
	m_Stages[m_CurrentStage]->draw(renderer);
	samus.draw(renderer);

	sporeSpawn.draw(renderer);

	Physics::debugDraw(renderer);
}

void Game::drawUI(Renderer& renderer)
{
	playerHUD.draw(renderer);
}

#include "Game.h"

// Declaring objects
Camera camera(20.0f);
Samus samus;
StageHub hubStage;
StageSporeSpawn sporeSpawnStage;
HUD playerHUD;

SporeSpawn sporeSpawn;

float movementSpeed = 0.5f;

Game::Game()
{
}

Game::~Game()
{
}

// Begin function
void Game::Begin(const sf::Window& window)
{
	Physics::init();

	//load map image
	sf::Image image;
	image.loadFromFile(m_CurrentStage);

	camera.position = sf::Vector2f(image.getSize().x / 2, image.getSize().y / 2);

	// render map and return initial samus and boss position
	std::vector<sf::Vector2f> mapPositions;
	mapPositions = sporeSpawnStage.createFromImg(image);
	samus.position = mapPositions[0];
	sporeSpawn.position = mapPositions[1];
	samus.begin();

	playerHUD.begin();

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
	// map.setBackground(renderer, "background_Eg2.png", sf::Vector2f(1920, 1080));

	sporeSpawnStage.draw(renderer);
	samus.draw(renderer);

	sporeSpawn.draw(renderer);

	Physics::debugDraw(renderer);
}

void Game::drawUI(Renderer& renderer)
{
	playerHUD.draw(renderer);
}

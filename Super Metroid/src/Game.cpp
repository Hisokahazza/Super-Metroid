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

void Game::createMenus()
{
	m_Menus[VICTORY] = new Victory();
	m_Menus[GAMEOVER] = new GameOver();
	m_Menus[NOMENU] = nullptr;
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
	createMenus();

	Physics::init();

	//load map image
	m_MapImage.loadFromFile(m_CurrentStage);

	camera.position = sf::Vector2f(m_MapImage.getSize().x / 2.0f, m_MapImage.getSize().y / 2.0f);
	camera.scaleView = sf::Vector2f(1.0f, 1.0f);

	// render map and return initial samus and boss position
	m_MapPositions = m_Stages[m_CurrentStage]->createFromImg(m_MapImage);
	
	samus.position = m_MapPositions[0];
	samus.begin();

	playerHUD.begin();

	if (m_Menus[m_CurrentMenuState] != nullptr)
	{
		m_Menus[m_CurrentMenuState]->begin();
	}

	sporeSpawn.position = m_MapPositions[1];
	sporeSpawn.begin();
}

// update function(called every frame)
void Game::update(float deltaTime)
{
	if (sporeSpawn.switchScreens == true)
	{
		m_CurrentMenuState = VICTORY;
	}

	if (m_Menus[m_CurrentMenuState] != nullptr)
	{
		if (m_Menus[m_CurrentMenuState]->returnToHub == true)
		{
			m_CurrentMenuState = NOMENU;
			//m_CurrentStage = "res/Level_Hub.png";
		}
	}
	
	if (m_Menus[m_CurrentMenuState] != nullptr)
	{
		m_Menus[m_CurrentMenuState]->update(deltaTime);
	}
	
	Physics::update(deltaTime);
	samus.update(deltaTime);

	sporeSpawn.update(deltaTime);
}

// Final rendering step
void Game::draw(Renderer& renderer)
{
	m_Stages[m_CurrentStage]->draw(renderer);
	samus.draw(renderer);

	sporeSpawn.draw(renderer);

	Physics::debugDraw(renderer);
}

void Game::drawUI(Renderer& renderer)
{
	playerHUD.draw(renderer);
}

void Game::drawMenu(Renderer& renderer)
{
	if (m_Menus[m_CurrentMenuState] != nullptr)
	{
		m_Menus[m_CurrentMenuState]->draw(renderer);
	}
}

#include "Game.h"

// Declaring objects
Camera camera(20.0f);
Samus samus;
HUD playerHUD;
MenuManager menuManager;

SporeSpawn sporeSpawn;

float movementSpeed = 0.5f;

void Game::createStages()
{
	m_Stages["res/Level_Hub.png"] = new StageHub();
	m_Stages["res/Level_Spore_Spawn.png"] = new StageSporeSpawn();
}

void Game::setCurrentStage(std::string& currentStage)
{
	m_Stages[m_CurrentStage]->clearLevel();
	
	m_CurrentStage = currentStage;

	m_MapImage.loadFromFile(m_CurrentStage);
	m_MapPositions = m_Stages[m_CurrentStage]->createFromImg(m_MapImage);
	camera.position = sf::Vector2f(m_MapImage.getSize().x / 2.0f, m_MapImage.getSize().y / 2.0f);
	samus.position = m_MapPositions[0];
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
	menuManager.begin();

	createStages();

	Physics::init();

	//load map image
	setCurrentStage(m_CurrentStage);
	samus.begin();

	sporeSpawn.position = m_MapPositions[1];
	sporeSpawn.begin();
}

// update function(called every frame)
void Game::update(float deltaTime)
{
	if (menuManager.getSwitchScreen() != NOMENU)
	{
		menuManager.setMenued(true);
	}

	if (samus.checkSamusAlive() == true)
	{
		m_IsSamusAlive = false;
	}


	if (menuManager.menus[menuManager.getSwitchScreen()]->returnToHub == true)
	{
		menuManager.setSwitchScreen(NOMENU);

		if (menuManager.checkMenued() == true)
		{
			std::cout << "CLEARED" << std::endl;
			setCurrentStage(m_HubStage);
		}
	}
	
	menuManager.menus[menuManager.getSwitchScreen()]->update(deltaTime);

	if (menuManager.checkMenued() == true)
	{
		return;
	}
	
	Physics::update(deltaTime);
	samus.update(deltaTime);

	if (m_CurrentStage != "res/Level_Hub.png")
	{
		// determine which boss to play
		sporeSpawn.update(deltaTime);
	}
}

// Final rendering step
void Game::draw(Renderer& renderer)
{
	if (menuManager.checkMenued() == true)
	{
		return;
	}

	m_Stages[m_CurrentStage]->draw(renderer);
	samus.draw(renderer);

	if (m_CurrentStage != "res/Level_Hub.png")
	{
		sporeSpawn.draw(renderer);
	}
	
	Physics::debugDraw(renderer);
}

void Game::drawUI(Renderer& renderer)
{
	if (menuManager.checkMenued() == true)
	{
		return;
	}

	playerHUD.draw(renderer);
}

void Game::drawMenu(Renderer& renderer)
{
	menuManager.menus[menuManager.getSwitchScreen()]->draw(renderer);
}

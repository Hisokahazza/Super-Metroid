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
	m_Menus[NOMENU] = new NoMenu();
}

void Game::setCurrentStage(std::string& currentStage)
{
	m_Stages[m_CurrentStage]->clearLevel();

	m_CurrentStage = currentStage;

	m_MapImage.loadFromFile(currentStage);
	m_MapPositions = m_Stages[currentStage]->createFromImg(m_MapImage);
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
	createStages();
	createMenus();

	Physics::init();

	//load map image
	setCurrentStage(m_CurrentStage);
	samus.begin();

	m_Menus[VICTORY]->begin();
	m_Menus[GAMEOVER]->begin();

	sporeSpawn.position = m_MapPositions[1];
	sporeSpawn.begin();
}

// update function(called every frame)
void Game::update(float deltaTime)
{
	m_Menued = false;

	if (samus.checkSamusAlive() == true)
	{
		m_IsSamusAlive = false;
	}

	if (sporeSpawn.checkSwitchScreens() == true)
	{
		m_Menued = true;
		m_CurrentMenuState = VICTORY;
	}

	if (samus.checkSwitchScreens() == true)
	{
		m_Menued = true;
		m_CurrentMenuState = VICTORY;
	}

	if (m_Menus[m_CurrentMenuState]->returnToHub == true)
	{
		m_CurrentMenuState = NOMENU;
		sporeSpawn.setSwitchScreens(false);

		std::string hubStage = "res/Level_Hub.png";
		setCurrentStage(hubStage);
	}
	
	m_Menus[m_CurrentMenuState]->update(deltaTime);

	if (m_Menued == true)
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
	if (m_Menued)
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
	if (m_Menued == true)
	{
		return;
	}

	playerHUD.draw(renderer);
}

void Game::drawMenu(Renderer& renderer)
{
	m_Menus[m_CurrentMenuState]->draw(renderer);
}

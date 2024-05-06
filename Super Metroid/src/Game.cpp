#include "Game.h"

// Declaring objects
Camera camera(20.0f);
Samus samus;
HUD playerHUD;
MenuManager menuManager;

//SporeSpawn sporeSpawn;

float movementSpeed = 0.5f;

void Game::createStages()
{
	m_Stages["res/Level_Hub.png"] = new StageHub();
	m_Stages["res/Level_Spore_Spawn.png"] = new StageSporeSpawn();
	m_Stages["res/Level_Gold_Torizo.png"] = new StageGoldTorizo();
}

void Game::createBosses()
{
	m_Bosses[SPORESPAWN] = new SporeSpawn();
	m_Bosses[GOLDTORIZO] = new GoldTorizo();
}

void Game::setCurrentStage(std::string& currentStage, bool initStage)
{
	m_Bosses[m_CurrentBoss]->resetFixture();

	m_Stages[m_CurrentStage]->clearLevel();
	
	m_CurrentStage = currentStage;

	m_MapImage.loadFromFile(currentStage);
	m_MapPositions = m_Stages[currentStage]->createFromImg(m_MapImage);
	samus.position = m_MapPositions[0];

	if (initStage == false)
	{
		samus.reset();
	}
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
	createBosses();

	Physics::init();

	//load map image
	setCurrentStage(m_CurrentStage, true);

	samus.setCurrentBoss(m_Bosses[m_CurrentBoss]);
	samus.begin();

	camera.position = samus.position;

	if (m_CurrentStage != m_HubStage)
	{
		m_Bosses[m_CurrentBoss]->position = m_MapPositions[1];
		m_Bosses[m_CurrentBoss]->begin();
	}
	
	//sporeSpawn.position = m_MapPositions[1]
	//sporeSpawn.begin();
}

// update function(called every frame)
void Game::update(float deltaTime)
{
	camera.position = samus.position;

	if (samus.checkSamusAlive() == true)
	{
		m_IsSamusAlive = false;
	}
	
	if (menuManager.menus[menuManager.getSwitchScreen()]->returnToHub == true)
	{
		std::cout << menuManager.getSwitchScreen() << std::endl;
		menuManager.setSwitchScreen(NOMENU);
		menuManager.menus[menuManager.getSwitchScreen()]->returnToHub = false;

		setCurrentStage(m_HubStage, false);
	}

	menuManager.menus[menuManager.getSwitchScreen()]->update(deltaTime);
	
	Physics::update(deltaTime);
	samus.update(deltaTime);

	if (m_CurrentStage == m_HubStage)
	{
		m_InteractableDoors = m_Stages[m_CurrentStage]->getDoors();
		for (auto door : m_InteractableDoors)
		{
			m_IsThroughDoor = door->getIsThroughDoor();
			door->Update(deltaTime);

			if (m_IsThroughDoor == true)
			{
				menuManager.setMenued(true);
				menuManager.setSwitchScreen(door->getDoorLink());

				if (menuManager.menus[door->getDoorLink()]->returnToHub == true)
				{
					door->setIsThroughDoor(false);
				}
			}
		}
	}

	if (m_CurrentStage != m_HubStage)
	{
		// determine which boss to play
		m_Bosses[m_CurrentBoss]->update(deltaTime);
	}
}

// Final rendering step
void Game::draw(Renderer& renderer)
{
	m_Stages[m_CurrentStage]->draw(renderer);
	samus.draw(renderer);

	if (m_CurrentStage == m_HubStage)
	{
		for (auto door : m_InteractableDoors)
		{
			door->draw(renderer);
		}
	}

	if (m_CurrentStage != m_HubStage)
	{
		m_Bosses[m_CurrentBoss]->draw(renderer);
	}
	
	Physics::debugDraw(renderer);
}

void Game::drawUI(Renderer& renderer)
{
	playerHUD.draw(renderer);
}

void Game::drawMenu(Renderer& renderer)
{
	menuManager.menus[menuManager.getSwitchScreen()]->draw(renderer);
}

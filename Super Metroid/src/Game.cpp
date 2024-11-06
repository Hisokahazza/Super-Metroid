#include "Game.h"

// Declaring objects
Camera camera(20.0f);
Samus samus;
HUD playerHUD;
MenuManager menuManager;

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
	std::cout << m_CurrentStage << std::endl;

	m_Bosses[m_CurrentBoss]->resetFixture();
	m_Bosses[m_CurrentBoss]->reset();

	m_Stages[m_CurrentStage]->clearLevel();
	
	m_CurrentStage = currentStage;

	m_MapImage.loadFromFile(currentStage);
	m_MapPositions = m_Stages[currentStage]->createFromImg(m_MapImage);
	samus.position = m_MapPositions[0];

	m_Bosses[m_CurrentBoss]->position = m_MapPositions[1];
	m_Bosses[m_CurrentBoss]->begin();
	samus.setCurrentBoss(m_Bosses[m_CurrentBoss]);
	
	
	if (initStage == false)
	{
		samus.reset();
	}
}

void Game::setCurrentBoss(BossName boss)
{
	m_CurrentBoss = boss;
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

	//load map image
	setCurrentStage(m_CurrentStage, true);

	// Handle samus object startup
	samus.setCurrentBoss(m_Bosses[m_CurrentBoss]);
	samus.begin();

	// set initial camera position to initial samus position
	camera.position = samus.position;

	if (m_CurrentStage != m_HubStage)
	{
		m_Bosses[m_CurrentBoss]->position = m_MapPositions[1];
		m_Bosses[m_CurrentBoss]->begin();
	}

	m_BossMenu = (BossMenu*)menuManager.menus[BOSSMENU];
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
		menuManager.menus[menuManager.getSwitchScreen()]->returnToHub = false;
		menuManager.setSwitchScreen(NOMENU);
		
		setCurrentStage(m_HubStage, false);
	}


	menuManager.menus[menuManager.getSwitchScreen()]->update(deltaTime);
	
	Physics::update(deltaTime);

	samus.update(deltaTime);

	switch (m_BossMenu->getSelectedBossItem())
	{
	case 0:
		break;
	case 1:
		m_BossMenu->setSelectedBossItem(0);
		m_BossMenu->setBossSelectMenuOpen(false);

		menuManager.setSwitchScreen(NOMENU);
		menuManager.setMenued(false);

		setCurrentBoss(SPORESPAWN);
		setCurrentStage(m_SporeSpawnStage, false);
		break;
	case 2:
		m_BossMenu->setSelectedBossItem(0);
		m_BossMenu->setBossSelectMenuOpen(false);

		menuManager.setSwitchScreen(NOMENU);
		menuManager.setMenued(false);

		setCurrentBoss(GOLDTORIZO);
		setCurrentStage(m_GoldTorizoStage, false);
		break;
	}

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

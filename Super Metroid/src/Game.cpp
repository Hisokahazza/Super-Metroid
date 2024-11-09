#include "Game.h"

// Declaring objects
Camera camera(20.0f);
Samus samus;
HUD playerHUD;
MenuManager menuManager;

float movementSpeed = 0.5f;

void Game::createStages()
{
	// Initialise stage objects
	m_Stages["res/Level_Hub.png"] = new StageHub();
	m_Stages["res/Level_Spore_Spawn.png"] = new StageSporeSpawn();
	m_Stages["res/Level_Gold_Torizo.png"] = new StageGoldTorizo();
}

void Game::createBosses()
{
	// Initialise boss objects
	m_Bosses[SPORESPAWN] = new SporeSpawn();
	m_Bosses[GOLDTORIZO] = new GoldTorizo();
}

void Game::setCurrentStage(std::string& currentStage, bool initStage)
{
	// Reset fixtures and bosses inbetween bosses
	if (m_BossRushQueue.empty() == true && m_CurrentStage != m_HubStage)
	{
		m_Bosses[m_CurrentBoss]->resetFixture();
		m_Bosses[m_CurrentBoss]->reset();
	}

	m_Stages[m_CurrentStage]->clearLevel();

	m_CurrentStage = currentStage;

	// Handle loading of new stage
	m_MapImage.loadFromFile(currentStage);
	m_MapPositions = m_Stages[currentStage]->createFromImg(m_MapImage);
	samus.position = m_MapPositions[0];

	// Handle startup of a new boss
	if (m_IsFirstBoss == false && m_CurrentStage != m_HubStage)
	{
		m_Bosses[m_CurrentBoss]->position = m_MapPositions[1];
		m_Bosses[m_CurrentBoss]->begin();
		samus.setCurrentBoss(m_Bosses[m_CurrentBoss]);
	}

	// Ensures no boss fixture is created before a boss stage is loaded as m_CurrentBoss has a default value
	m_IsFirstBoss = false;
	
	// Reset samus assuming it is not the first stage created
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
void Game::begin(const sf::Window& window)
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

	// Initialise boss stages map to translate from c style string in menu to boss name enum
	m_BossStages = { {SPORESPAWN, m_SporeSpawnStage}, {GOLDTORIZO, m_GoldTorizoStage} };
	// Initialise general Menu object and cast to BossMenu*
	m_BossMenu = (BossMenu*)menuManager.menus[BOSSMENU];
}

// update function(called every frame)
void Game::update(float deltaTime)
{
	camera.position = samus.position;

	if (samus.checkSamusAlive() == true)
	{
		m_IsSamusDead = false;
	}
	else
	{
		m_IsSamusDead = true;
	}
	
	// Check if player should retyurn to hub stage every frame
	if (menuManager.menus[menuManager.getSwitchScreen()]->returnToHub == true)
	{
		menuManager.menus[menuManager.getSwitchScreen()]->returnToHub = false;
		menuManager.setSwitchScreen(NOMENU);
		
		setCurrentStage(m_HubStage, false);
	}

	menuManager.menus[menuManager.getSwitchScreen()]->update(deltaTime);
	
	Physics::update(deltaTime);

	samus.update(deltaTime);

	// Reset boss rush queue when single boss is selected
	if (m_BossMenu->getSelectedBossItem() > 0)
	{
		m_BossRushQueue = {};
		m_BossMenu->resetBossRushSelections();
	}

	// Reset boss rush queue when samus is dead
	if (m_IsSamusDead == true)
	{
		m_BossRushQueue = {};
		m_BossMenu->resetBossRushSelections();

		m_IsGameOver = true;
	}
	
	// Handle single boss selection
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

	// Handle door entrance
	if (m_CurrentStage == m_HubStage)
	{
		m_InteractableDoors = m_Stages[m_CurrentStage]->getDoors();
		// For every door check for entrance and menu appropriately
		for (auto door : m_InteractableDoors)
		{
			m_IsThroughDoor = door->getIsThroughDoor();
			door->update(deltaTime);

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

	// Push boss rush elements to a queue when boss rush start button is pressed
	if (m_BossMenu->getIsBossRushStarted() == true)
	{
		// Interface between bossName enum and boss strings used in menu class
		for (auto boss : m_BossMenu->getBossRushSelections())
		{
			if (boss == "Spore Spawn")
			{
				m_BossRushQueue.push(SPORESPAWN);
			}
			else if (boss == "Gold Torizo")
			{
				m_BossRushQueue.push(GOLDTORIZO);
			}
		}

		m_BossMenu->setIsBossRushStarted(false);
		m_BossMenu->resetBossRushSelections();
		m_BossMenu->setBossSelectMenuOpen(false);
	}

	// Handle boss rush
	if (m_BossRushQueue.empty() == false && m_IsBossInRushActive == false)
	{
		// Reset fixtures and bosses inbetween bosses
		if (m_CurrentStage != m_HubStage)
		{
			m_Bosses[m_LastBossInRush]->resetFixture();
			m_Bosses[m_LastBossInRush]->reset();
		}

		// Assign boss previous to current to a variable
		m_LastBossInRush = m_BossRushQueue.front();

		menuManager.setSwitchScreen(NOMENU);
		menuManager.setMenued(false);

		setCurrentBoss(m_BossRushQueue.front());
		setCurrentStage(m_BossStages[m_BossRushQueue.front()], false);

		// Pop boss from queue once boss is fully started
		m_BossRushQueue.pop();

		m_IsBossInRushActive = true;
	}

	// Set boss rush queue size in boss file so that it knows whether or not to display victory screen 
	m_Bosses[m_CurrentBoss]->setBossRushQueueSize(m_BossRushQueue.size());

	if (m_Bosses[m_CurrentBoss]->getIsBossComplete() == true || m_IsSamusDead == true)
	{
		m_IsBossInRushActive = false;
	}

	if (m_CurrentStage != m_HubStage)
	{
		// determine which boss to play
		m_Bosses[m_CurrentBoss]->update(deltaTime);
	}
}

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

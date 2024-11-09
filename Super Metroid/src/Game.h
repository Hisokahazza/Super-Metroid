#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <Queue>

#include "Renderer.h"
#include "Resources.h"
#include "Camera.h"
#include "Level.h"
#include "Samus.h"
#include "Boss.h"
#include "HUD.h"
#include "Menu.h"
#include "MenuManager.h"

extern Camera camera;

class Game
{
private:
	std::string m_CurrentStage = "res/Level_Hub.png";
	BossName m_CurrentBoss = GOLDTORIZO;

	bool m_IsSamusDead = false;

	void createStages();
	void createBosses();

	void setCurrentStage(std::string& currentStage, bool initStage);
	void setCurrentBoss(BossName boss);

	std::unordered_map<std::string, Level*> m_Stages;
	std::unordered_map<BossName, Boss*> m_Bosses;

	sf::Image m_MapImage;
	std::vector<sf::Vector2f> m_MapPositions;
	std::vector<Door*> m_InteractableDoors;

	bool m_IsThroughDoor;

	std::string m_HubStage = "res/Level_Hub.png";
	std::string m_GoldTorizoStage = "res/Level_Gold_Torizo.png";
	std::string m_SporeSpawnStage = "res/Level_Spore_Spawn.png";

	std::unordered_map<BossName, std::string> m_BossStages;

	BossMenu* m_BossMenu;
	std::queue<BossName> m_BossRushQueue;

	bool m_IsBossInRushActive = false;
	bool m_IsFirstBoss = true;
	bool m_IsGameOver = false;
	BossName m_LastBossInRush = GOLDTORIZO;
public:
	Game();
	~Game();

	void begin(const sf::Window& window);
	void update(float deltaTime);

	void draw(Renderer& renderer);
	void drawUI(Renderer& renderer);
	void drawMenu(Renderer& renderer);
};



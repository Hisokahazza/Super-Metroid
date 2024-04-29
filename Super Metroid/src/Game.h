#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>

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
	std::string m_CurrentStage = "res/Level_Spore_Spawn.png";

	bool m_IsSamusAlive = true;

	void createStages();

	void setCurrentStage(std::string& currentStage);

	std::unordered_map<std::string, Level*> m_Stages;

	sf::Image m_MapImage;
	std::vector<sf::Vector2f> m_MapPositions;

	std::string m_HubStage = "res/Level_Hub.png";
public:
	Game();
	~Game();

	void Begin(const sf::Window& window);
	void update(float deltaTime);

	void draw(Renderer& renderer);
	void drawUI(Renderer& renderer);
	void drawMenu(Renderer& renderer);
};



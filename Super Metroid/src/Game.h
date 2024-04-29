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

extern Camera camera;

class Game
{
private:
	std::string m_CurrentStage = "res/Level_Spore_Spawn.png";
	MenuState m_CurrentMenuState = NOMENU;

	bool m_Menued = false;

	void createStages();
	void createMenus();

	void setCurrentStage(std::string& currentStage);

	std::unordered_map<std::string, Level*> m_Stages;
	std::unordered_map<MenuState, Menu*> m_Menus;

	sf::Image m_MapImage;
	std::vector<sf::Vector2f> m_MapPositions;
public:
	Game();
	~Game();

	void Begin(const sf::Window& window);
	void update(float deltaTime);

	void draw(Renderer& renderer);
	void drawUI(Renderer& renderer);
	void drawMenu(Renderer& renderer);
};



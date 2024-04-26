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

extern Camera camera;

class Game
{
private:
	std::string m_CurrentStage = "res/Level_Hub.png";
public:
	Game();
	~Game();

	void Begin(const sf::Window& window);
	void update(float deltaTime);
	void draw(Renderer& renderer);
	void drawUI(Renderer& renderer);
};


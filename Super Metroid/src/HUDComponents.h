#pragma once

#include "Renderer.h"
#include "Projectile.h"
#include "Resources.h"
#include "Animation.h"

class EnergyBar
{
private:
	sf::Text m_EnergyText;
	sf::Text m_EnergyNum;

	int m_EnergyValue;

	int m_TotalEnergyTanks;
	int m_NumActiveEnergyTanks;
public:
	void begin(int initialEnergyTanks);
	void update(float deltaTime, int healthOffset);
	void draw(Renderer& renderer);
};

class Ammo
{
private:
	sf::Text m_AmmoNum;

	int m_AmmoValue;
	ProjectileType m_ActiveProjectile;
public:
	void begin();
	void update(float deltaTime, int ammoOffset, ProjectileType activeProjectile);
	void draw(Renderer& renderer);
};

class MiniMap
{
private:
	std::vector<sf::Texture> m_FlashTextures;
	SheetlessAnimation* m_MiniMapFlash;

	std::vector<std::vector<int>> m_Grid = { {1, 1, 1, 1, 1}, 
											 {1, 1, 0, 1, 1},
											 {1, 1, 1, 1, 1} };
public:
	void begin();
	void update(float deltaTime);
	void draw(Renderer& renderer);
};
#pragma once

#include "HUDComponents.h"

class HUD
{
private:
	sf::RectangleShape m_HUDbar;

	EnergyBar m_EnergyBar;
	Ammo m_AmmoHUD;
	MiniMap m_Minimap;
public:
	void begin(int initialEnergyTanks);
	void update(float deltaTime, int healthOffset, int ammoOffset, ProjectileType activeProjectile);
	void draw(Renderer& renderer);
	void reset();
};
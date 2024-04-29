#include "HUD.h"

void HUD::begin(int initialEnergyTanks)
{
	m_EnergyBar.begin(initialEnergyTanks);
	m_AmmoHUD.begin();
	m_Minimap.begin();
}

void HUD::update(float deltaTime, int healthOffset, int ammoOffset, ProjectileType activeProjectile)
{
	m_EnergyBar.update(deltaTime, healthOffset);
	m_AmmoHUD.update(deltaTime, ammoOffset, activeProjectile);
	m_Minimap.update(deltaTime);
}

void HUD::draw(Renderer& renderer)
{
	// Render bar which HUD elements reside on
	renderer.drawRectangleShape(m_HUDbar, sf::Vector2f(-50.0f, -28.5f), sf::Vector2f(100.0f, 6.0f), sf::Color::Black);

	m_EnergyBar.draw(renderer);
	m_AmmoHUD.draw(renderer);
	m_Minimap.draw(renderer);
}

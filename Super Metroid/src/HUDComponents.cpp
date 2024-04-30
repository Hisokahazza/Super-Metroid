#include "HUDComponents.h"

void EnergyBar::begin(int initialEnergyTanks)
{
	// Init base values
	m_TotalEnergyTanks = initialEnergyTanks;
	m_NumActiveEnergyTanks = initialEnergyTanks;
	m_EnergyValue = 99;
}

void EnergyBar::update(float deltaTime, int healthOffset)
{
	// Update health based on response from samus class
	m_EnergyValue += healthOffset;

	if (m_EnergyValue <= 0 && m_NumActiveEnergyTanks <= 0)
	{
		m_EnergyValue = 0;
	}

	// Reset health when energy tank runs out
	if (m_EnergyValue <= 0 && m_NumActiveEnergyTanks > 0)
	{
		m_EnergyValue += 99;
		m_NumActiveEnergyTanks--;
	}
}

void EnergyBar::draw(Renderer& renderer)
{
	// Render energy text
	std::string energyText = "ENERGY";
	renderer.drawText(m_EnergyText, sf::Vector2f(-47.5f, -23.5f), sf::Vector2f(0.025f, 0.03f), energyText);

	// Render energy value based on member variable (m_EnergyValue)
	std::string energyNumText = std::to_string(m_EnergyValue);
	renderer.drawText(m_EnergyNum, sf::Vector2f(-39.0f, -23.5f), sf::Vector2f(0.025f, 0.03f), energyNumText);

	// Render active tanks by rendering one for each active tank
	for (int count = 0; count < m_NumActiveEnergyTanks; count++)
	{
		renderer.draw(Resources::textures["Energy_Bar_Active.png"], sf::Vector2f((count * 1.5f) + -47.0f, -25.0f), sf::Vector2f(1.5f, 1.5f));
	}

	// Render inactive tanks by iterating from total to active tanks (-1 is there to ensure individual inactive bars are rendered in the correct positions as the orient around the count)
	for (int count = m_TotalEnergyTanks - 1; count >= m_NumActiveEnergyTanks; count--)
	{
		renderer.draw(Resources::textures["Energy_Bar_InActive.png"], sf::Vector2f((count * 1.5f) + -47.0f, -25.0f), sf::Vector2f(1.5f, 1.5f));
	}
}

void EnergyBar::reset()
{
	m_NumActiveEnergyTanks = m_TotalEnergyTanks;
	m_EnergyValue = 99;
}

void Ammo::begin()
{
	// Init base value
	m_AmmoValue = 15;
}

void Ammo::update(float deltaTime, int ammoOffset, ProjectileType activeProjectile)
{
	// Update ammo based on offset and set the currently active projectile based on the response from the samus class 
	m_AmmoValue += ammoOffset;
	m_ActiveProjectile = activeProjectile;
}

void Ammo::draw(Renderer& renderer)
{
	// Render number of ammo based on member variable (m_AmmoValue)
	std::string ammoNumText = "0" + std::to_string(m_AmmoValue);
	renderer.drawText(m_AmmoNum, sf::Vector2f(-25.0f, -23.5f), sf::Vector2f(0.025f, 0.03f), ammoNumText);

	// Render either inactive or active icon dependent on player selected ammo type
	if (m_ActiveProjectile == BULLETPROJ)
	{
		renderer.draw(Resources::textures["Missile_Icon_InActive.png"], sf::Vector2f(-23.0f, -26.0f), sf::Vector2f(6.0f, 4.0f));
	}
	
	if (m_ActiveProjectile == MISSILEPROJ)
	{
		renderer.draw(Resources::textures["Missile_Icon_Active.png"], sf::Vector2f(-23.0f, -26.0f), sf::Vector2f(6.0f, 4.0f));
	}

}

void Ammo::reset()
{
	m_AmmoValue = 15;
}

void MiniMap::begin()
{
	m_FlashTextures = { Resources::textures["Mini_Map_S_WP.png"], Resources::textures["Mini_Map_S_RO.png"] };
	m_MiniMapFlash = new SheetlessAnimation(m_FlashTextures, 0.3f);
	m_MiniMapFlash->begin();
}

void MiniMap::update(float deltaTime)
{
	m_MiniMapFlash->update(deltaTime);
}

void MiniMap::draw(Renderer& renderer)
{
	for (int row = 0; row < m_Grid.size(); row++) {
		for (int column = 0; column < m_Grid[row].size(); column++)
		{
			if (m_Grid[row][column] == 0)
			{
				renderer.draw(m_MiniMapFlash->getCurrentFrame(), sf::Vector2f(37.5f + column * 1.25, -26.5f + row * 1.25), sf::Vector2f(1.5f, 1.5f));
			}
			if (m_Grid[row][column] == 1)
			{
				renderer.draw(Resources::textures["Mini_Map_Component.png"], sf::Vector2f(37.5f + column * 1.25, -26.5f + row * 1.25), sf::Vector2f(1.0f , 1.0f));
			}
		}
	}
}

void MiniMap::reset()
{
}

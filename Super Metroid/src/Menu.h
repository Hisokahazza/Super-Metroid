#pragma once

#include "Renderer.h"
#include "Resources.h"
#include "Animation.h"
#include <array>

enum MenuState
{
	NOMENU,
	VICTORY,
	GAMEOVER,
	BOSSMENU
};

enum MenuButton
{
	// Boss select menu buttons
	BOSSSELECT,
	EXIT
};

class Menu
{
private:
public:
	bool returnToHub = false;

	virtual void begin() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw(Renderer& renderer) = 0;
};

class Victory : public Menu
{
private:
	sf::Text m_Victorytext;
	std::vector<sf::Texture> m_ReturnTextures;
	SheetlessAnimation* m_ReturnTextAnim;
public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;
};

class GameOver : public Menu
{
private:
	sf::Text m_GameOvertext;
	std::vector<sf::Texture> m_ReturnTextTextures;
	SheetlessAnimation* m_ReturnTextAnim;
public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;
};

class BossMenu : public Menu
{
private:
	std::vector<sf::Texture> m_BossSelectTextTextures;
	std::vector<sf::Texture> m_ExitTextTextures;

	SheetlessAnimation* m_BossSelectTextAnim;
	SheetlessAnimation* m_ExitTextAnim;

	unsigned int m_CurrentButtonIndex = 0;
	int m_SelectedBossItem;
	std::vector<MenuButton> m_Buttons;

	bool m_BossSelectMenuOpen = false;
	bool startBoss = false;
public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;

	// Getters and setters
	int const getSelectedBossItem() { return m_SelectedBossItem; }
	void const setSelectedBossItem(int selectedBossItem) { m_SelectedBossItem = selectedBossItem; }
	void const setBossSelectMenuOpen(bool bossSelectMenuOpen) { m_BossSelectMenuOpen = bossSelectMenuOpen; }
};

class NoMenu : public Menu
{
private:
public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;
};
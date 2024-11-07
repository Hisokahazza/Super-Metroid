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
	int m_SelectedBossRushItem;
	std::vector<MenuButton> m_Buttons;
	std::vector<const char*> m_BossRushSelections;

	int m_ButtonLine = 0;

	bool m_IsBossSelectMenuOpen = false;
	bool m_IsBossRushStarted;
public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;

	void resetBossRushSelections();

	// Getters and setters
	int const getSelectedBossItem() { return m_SelectedBossItem; }
	void const setSelectedBossItem(int selectedBossItem) { m_SelectedBossItem = selectedBossItem; }

	void const setBossSelectMenuOpen(bool bossSelectMenuOpen) { m_IsBossSelectMenuOpen = bossSelectMenuOpen; }

	bool const getIsBossRushStarted() { return m_IsBossRushStarted; };
	void const setIsBossRushStarted(bool isBossRushStarted) { m_IsBossRushStarted = isBossRushStarted; };

	std::vector<const char*> const getBossRushSelections() { return m_BossRushSelections; };
};

class NoMenu : public Menu
{
private:
public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;
};
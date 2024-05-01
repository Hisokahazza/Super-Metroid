#pragma once

#include "Renderer.h"
#include "Resources.h"
#include "Animation.h"

enum MenuState
{
	NOMENU,
	VICTORY,
	GAMEOVER,
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
	std::vector<sf::Texture> m_ReturnTextures;
	SheetlessAnimation* m_ReturnTextAnim;
public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;
};

class NoMenu : public Menu
{
private:
public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;
};
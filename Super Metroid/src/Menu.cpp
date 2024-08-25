#include "Menu.h"

// Handles victory menu rendering
void Victory::begin()
{
	m_ReturnTextures = {
		Resources::textures["Return_Text_01.png"],
		Resources::textures["Return_Text_02_Green.png"]
	};

	m_ReturnTextAnim = new SheetlessAnimation(m_ReturnTextures, 0.3f);
	m_ReturnTextAnim->begin();
}

void Victory::update(float deltaTime)
{
	m_ReturnTextAnim->update(deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		returnToHub = true;
	}
}

void Victory::draw(Renderer& renderer)
{
	renderer.draw(Resources::textures["Victory_Screen.png"], sf::Vector2f(0, 0), sf::Vector2f(100.0f, 56.2f));

	std::string victoryTextContents = "VICTORY";
	renderer.drawText(m_Victorytext, sf::Vector2f(-10.0f, -20.0f), sf::Vector2f(0.05f, 0.06f), victoryTextContents);

	renderer.draw(m_ReturnTextAnim->getCurrentFrame(), sf::Vector2f(35.0f, 26.0f), sf::Vector2f(25.0f, 3.0f));
}

// Handles gameover menu rendering
void GameOver::begin()
{
	m_ReturnTextures = {
		Resources::textures["Return_Text_01.png"],
		Resources::textures["Return_Text_02_Red.png"]
	};

	m_ReturnTextAnim = new SheetlessAnimation(m_ReturnTextures, 0.3f);
	m_ReturnTextAnim->begin();
}

void GameOver::update(float deltaTime)
{
	m_ReturnTextAnim->update(deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		returnToHub = true;
	}
}

void GameOver::draw(Renderer& renderer)
{
	renderer.draw(Resources::textures["Game_Over.jpeg"], sf::Vector2f(0, 0), sf::Vector2f(100.0f, 56.2f));

	std::string gameOverTextContents = "GAME OVER";
	renderer.drawText(m_GameOvertext, sf::Vector2f(-10.0f, -25.0f), sf::Vector2f(0.05f, 0.06f), gameOverTextContents);

	renderer.draw(m_ReturnTextAnim->getCurrentFrame(), sf::Vector2f(35.0f, 26.0f), sf::Vector2f(25.0f, 3.0f));
}

// empty menu class to be assigned when no menu is active
void NoMenu::begin()
{
}

void NoMenu::update(float deltaTime)
{
}

void NoMenu::draw(Renderer& renderer)
{
}

// Handles boss menu rendering
void BossMenu::begin()
{

}

void BossMenu::update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		returnToHub = true;
	}
}

void BossMenu::draw(Renderer& renderer)
{
	renderer.draw(Resources::textures["Boss_Menu.png"], sf::Vector2f(0, 0), sf::Vector2f(100.0f, 56.2f));
}

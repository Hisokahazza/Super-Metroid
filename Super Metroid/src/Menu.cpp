#include "Menu.h"

void Victory::begin()
{
	m_ReturnTextures = {
		Resources::textures["Return_Text_01.png"],
		Resources::textures["Return_Text_02.png"]
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
	renderer.draw(Resources::textures["Victory_Screen.png"], sf::Vector2f(0, 0), sf::Vector2f(99.8f, 56.2f));

	std::string victoryText = "VICTORY";
	renderer.drawText(m_Victorytext, sf::Vector2f(-10.0f, -20.0f), sf::Vector2f(0.05f, 0.06f), victoryText);

	renderer.draw(m_ReturnTextAnim->getCurrentFrame(), sf::Vector2f(35.0f, 26.0f), sf::Vector2f(25.0f, 3.0f));
}

void GameOver::begin()
{
	m_ReturnTextures = {
		Resources::textures["Return_Text_01.png"],
		Resources::textures["Return_Text_02.png"]
	};

	m_ReturnTextAnim = new SheetlessAnimation(m_ReturnTextures, 0.3f);
	m_ReturnTextAnim->begin();
}

void GameOver::update(float deltaTime)
{
}

void GameOver::draw(Renderer& renderer)
{
}

void NoMenu::begin()
{
}

void NoMenu::update(float deltaTime)
{
}

void NoMenu::draw(Renderer& renderer)
{
}

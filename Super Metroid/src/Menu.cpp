#include "Menu.h"
#include <iostream>

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
	m_ReturnTextTextures = {
		Resources::textures["Return_Text_01.png"],
		Resources::textures["Return_Text_02_Red.png"]
	};

	m_ReturnTextAnim = new SheetlessAnimation(m_ReturnTextTextures, 0.3f);
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
	m_BossSelectTextTextures = 
	{
		Resources::textures["BossSelectText_01.png"],
		Resources::textures["BossSelectText_02.png"]
	};
	m_ExitTextTextures =
	{
		Resources::textures["ExitText_01.png"],
		Resources::textures["ExitText_02.png"]
	};

	m_BossSelectTextAnim = new SheetlessAnimation(m_BossSelectTextTextures, 0.3f);
	m_ExitTextAnim = new SheetlessAnimation(m_ExitTextTextures, 0.3f);

	m_BossSelectTextAnim->begin();
	m_ExitTextAnim->begin();

	m_Buttons =
	{
		BOSSSELECT,
		EXIT
	};
}

void BossMenu::update(float deltaTime)
{
	std::cout << m_CurrentButtonIndex << std::endl;

	m_BossSelectTextAnim->update(deltaTime);
	m_ExitTextAnim->update(deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (m_CurrentButtonIndex < m_Buttons.size() - 1)
		{
			m_CurrentButtonIndex++;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (m_CurrentButtonIndex > 0)
		{
			m_CurrentButtonIndex--;
		}
	}

	if (m_Buttons[m_CurrentButtonIndex] == EXIT && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		m_BossSelectMenuOpen = false;
		m_CurrentButtonIndex = 0;
		returnToHub = true;
	}
	else if (m_Buttons[m_CurrentButtonIndex] == BOSSSELECT && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		m_BossSelectMenuOpen = true;
	}

	if (m_BossSelectMenuOpen == true)
	{
		ImVec2 windowSize = ImVec2(1920, 1080);
		ImVec2 windowPos = ImVec2(0, 0);

		ImGui::SetNextWindowSize(windowSize);
		ImGui::SetNextWindowPos(windowPos);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.0f, 0.2f, 1.0f));

		// Begin window
		ImGui::Begin("Boss Selection Window");

		// Window contents
		const char* bossSelectionItems[]
		{ 
			"Choose a boss",
			"Spore Spawn",
			"Gold Torizo"
		};
		static int SelectedItem = 0;

		ImGui::BeginChild("##", ImVec2(ImGui::GetContentRegionAvail().x, 260), ImGuiChildFlags_None);
		ImGui::Text("Select A Boss");
		ImGui::Combo("##", &SelectedItem, bossSelectionItems, IM_ARRAYSIZE(bossSelectionItems));
		ImGui::EndChild();

		ImGui::PopStyleColor();
		
		// End window
		ImGui::End();

		m_SelectedBossItem = SelectedItem;
	}
}

void BossMenu::draw(Renderer& renderer)
{
	renderer.draw(Resources::textures["Boss_Menu.png"], sf::Vector2f(0, 0), sf::Vector2f(100.0f, 56.2f));

	if (m_Buttons[m_CurrentButtonIndex] == BOSSSELECT)
	{
		renderer.draw(m_BossSelectTextAnim->getCurrentFrame(), sf::Vector2f(0, -10.0f), sf::Vector2f(24.2f, 3.2f));
		renderer.draw(m_ExitTextTextures[0], sf::Vector2f(0, -5.0f), sf::Vector2f(11.7f, 3.2f));
	}
	else if (m_Buttons[m_CurrentButtonIndex] == EXIT)
	{
		renderer.draw(m_ExitTextAnim->getCurrentFrame(), sf::Vector2f(0, -5.0f), sf::Vector2f(11.7f, 3.2f));
		renderer.draw(m_BossSelectTextTextures[0], sf::Vector2f(0, -10.0f), sf::Vector2f(24.2f, 3.2f));
	}
}

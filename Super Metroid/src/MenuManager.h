#pragma once

#include "Menu.h"

class MenuManager
{
private:
	MenuState m_SwitchScreens = NOMENU;
	bool m_IsMenued = false;
	void createMenus();

public:
	std::unordered_map<MenuState, Menu*> menus;

	void begin();

	MenuState const getSwitchScreen() { return m_SwitchScreens; }
	void const setSwitchScreen(MenuState switchScreens) { m_SwitchScreens = switchScreens; }

	bool const checkMenued() { return m_IsMenued; }
	void const setMenued(bool isMenued) { m_IsMenued = isMenued; }
};
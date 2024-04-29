#include "MenuManager.h"

void MenuManager::createMenus()
{
	menus[VICTORY] = new Victory();
	menus[GAMEOVER] = new GameOver();
	menus[NOMENU] = new NoMenu();

	menus[VICTORY]->begin();
	menus[GAMEOVER]->begin();
}

void MenuManager::begin()
{
	createMenus();
}

#include "MenuManager.h"

void MenuManager::createMenus()
{
	menus[VICTORY] = new Victory();
	menus[GAMEOVER] = new GameOver();
	menus[BOSSMENU] = new BossMenu();
	menus[NOMENU] = new NoMenu();

	menus[VICTORY]->begin();
	menus[GAMEOVER]->begin();
	menus[BOSSMENU]->begin();
}

void MenuManager::begin()
{
	createMenus();
}

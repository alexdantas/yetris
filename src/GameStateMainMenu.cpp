#include "GameStateMainMenu.hpp"
#include "Ncurses.hpp"

GameStateMainMenu::GameStateMainMenu():
	layout(NULL)
{

}
GameStateMainMenu::~GameStateMainMenu()
{

}
void GameStateMainMenu::load(int stack)
{
	this->layout = new LayoutMainMenu(80, 24);
	this->layout->windowsInit();
}

int GameStateMainMenu::unload()
{
	this->layout->windowsExit();
	delete this->layout;

	return 0;
}

GameState::StateCode GameStateMainMenu::update()
{
	switch(Ncurses::getInput(100))
	{
	case 'q':
		return GameState::QUIT;

	case '\n':
		return GameState::GAME_START;
	}

	return GameState::CONTINUE;
}

void GameStateMainMenu::draw()
{
	this->layout->main->clear();

	this->layout->main->print("yetris", 1, 1, Colors::pair(COLOR_RED, COLOR_DEFAULT));

	this->layout->main->print(" _     ____ _____  ___   _   __  ", 1, 1, Colors::pair(COLOR_RED, COLOR_DEFAULT));
	this->layout->main->print("\\ \\_/ | |_   | |  | |_) | | ( (` ", 1, 2, Colors::pair(COLOR_RED, COLOR_DEFAULT));
	this->layout->main->print(" |_|  |_|__  |_|  |_| \\ |_| _)_) ", 1, 3, Colors::pair(COLOR_RED, COLOR_DEFAULT));


	this->layout->main->print("Press <enter> to start",
	                          1,
	                          this->layout->main->getH() - 2,
	                          Colors::pair(COLOR_BLUE, COLOR_DEFAULT, true));

	this->layout->main->refresh();
}



#include "Ncurses.hpp"
#include "Window.hpp"
#include "LayoutGame.hpp"
#include "Utils.hpp"
#include "GameModeSurvival.hpp"
#include "Globals.hpp"

int main(int argc, char *argv[])
{
	(void)(argc);
	(void)(argv);

	Globals::init();
	Utils::Random::seed();
	Ncurses::init();
	Colors::init();

	LayoutGame layout(80, 24);
	layout.windowsInit();

	GameModeSurvival game;
	game.start();

	while (! game.isOver())
	{
		game.handleInput(Ncurses::getInput(100));
		game.update();
		game.draw(&layout);
	}

	Ncurses::exit();
	return 0;
}


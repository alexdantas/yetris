#include "StateManager.hpp"
#include "Ncurses.hpp"
#include "Globals.hpp"
#include "Utils.hpp"
#include "Colors.hpp"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	try
	{
		Globals::init();
		Utils::Random::seed();
		Ncurses::init();
		Colors::init();

		StateManager states;
		states.run();

		Ncurses::exit();
	}
	catch (...)
	{
		// I dont really have a nice exception-handling scheme right
		// now. I must learn how to properly deal with them.
		Ncurses::exit();
		return 666;
	}

	return 0;
}


#include "Ncurses.hpp"
#include "Window.hpp"
#include "LayoutGame.hpp"

int main(int argc, char *argv[])
{
	(void)(argc);
	(void)(argv);

	Ncurses::init();
	Colors::init();

	LayoutGame layout(80, 24);
	layout.windowsInit();

	while (true)
	{
		int c = Ncurses::getInput(100);

		if (c == 'q')
			break;
	}

	Ncurses::exit();
	return 0;
}


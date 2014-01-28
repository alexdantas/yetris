#include "Ncurses.hpp"

int main(int argc, char *argv[])
{
	(void)(argc);
	(void)(argv);

	Ncurses::init();

	while (true)
	{
		int c = Ncurses::getInput(100);

		if (c == 'q')
			break;
	}

	Ncurses::exit();
	return 0;
}


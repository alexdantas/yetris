#include "Ncurses.hpp"
#include "Window.hpp"

int main(int argc, char *argv[])
{
	(void)(argc);
	(void)(argv);

	Ncurses::init();

	Window w(0, 0, 10, 5);
	w.print("Great", 3, 3);
	w.refresh();

	Window w2(&w, 2, 2, 8, 3);
	w2.print("Test", 0, 0);
	w2.refresh();

	while (true)
	{
		int c = Ncurses::getInput(100);

		if (c == 'q')
			break;
	}

	Ncurses::exit();
	return 0;
}


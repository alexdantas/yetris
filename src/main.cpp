#include "Ncurses.hpp"
#include "Window.hpp"

int main(int argc, char *argv[])
{
	(void)(argc);
	(void)(argv);

	Ncurses::init();
	Colors::init();

	Window w(0, 0, 20, 10);
	w.print("Great", 1, 1, Colors::pair(COLOR_RED, COLOR_DEFAULT));
	w.borders(Window::BORDER_REGULAR);
	w.refresh();

	Window w2(&w, 2, 3, 8, 3);
	w2.print("Test", 1, 1);
	w2.borders(Window::BORDER_FANCY);
	w2.clear();

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


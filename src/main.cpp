#include "Ncurses.hpp"
#include "Window.hpp"
#include "LayoutGame.hpp"
#include "Block.hpp"

int main(int argc, char *argv[])
{
	(void)(argc);
	(void)(argv);

	Ncurses::init();
	Colors::init();

	LayoutGame layout(80, 24);
	layout.windowsInit();

	Block block(Colors::pair(COLOR_RED, COLOR_DEFAULT, true),
	            '#', '#');
	block.draw(layout.board, 1, 0);
	layout.board->refresh();

	while (true)
	{
		int c = Ncurses::getInput(100);

		if (c == 'q')
			break;
	}

	Ncurses::exit();
	return 0;
}


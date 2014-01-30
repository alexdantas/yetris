#include "Ncurses.hpp"
#include "Window.hpp"
#include "LayoutGame.hpp"
#include "Piece.hpp"
#include "Globals.hpp"

int main(int argc, char *argv[])
{
	(void)(argc);
	(void)(argv);

	Globals::init();
	Ncurses::init();
	Colors::init();

	LayoutGame layout(80, 24);
	layout.windowsInit();

	Piece piece(Piece::PIECE_L, 1, 0);
	piece.draw(layout.board);
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


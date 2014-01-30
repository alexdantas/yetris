#include "Ncurses.hpp"
#include "Window.hpp"
#include "LayoutGame.hpp"
#include "Board.hpp"
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

	Board board(layout.board->getX(), layout.board->getY(),
	            DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT);

	Piece piece(Piece::PIECE_L, 4, 2);
	board.lockPiece(&piece);

	Piece piece2(Piece::PIECE_O, 0, 0);
	board.lockPiece(&piece2);

	board.draw(layout.board);
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


#include "Ncurses.hpp"
#include "Window.hpp"
#include "LayoutGame.hpp"
#include "Board.hpp"
#include "Globals.hpp"
#include "Utils.hpp"

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

	Board board(0, 0,
	            DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT);

	Piece piece(Piece::random(), 0, 0);

	bool willQuit = false;

	while (! willQuit)
	{
		int c = Ncurses::getInput(100);

		switch(c)
		{
		case 'q':
			willQuit = true;
			break;

		case 'a':
		{
			Piece tmp = piece;
			tmp.move(Piece::DIR_LEFT);

			if (board.isPieceValid(&tmp))
				piece.move(Piece::DIR_LEFT);

			break;
		}
		case 's':
		{
			Piece tmp = piece;
			tmp.move(Piece::DIR_DOWN);

			if (board.isPieceValid(&tmp))
				piece.move(Piece::DIR_DOWN);

			break;
		}
		case 'd':
		{
			Piece tmp = piece;
			tmp.move(Piece::DIR_RIGHT);

			if (board.isPieceValid(&tmp))
				piece.move(Piece::DIR_RIGHT);

			break;
		}
		case ' ':
		{
			board.lockPiece(&piece);

			piece = Piece(Piece::random(), 0, 0);
			break;
		}
		}

		layout.board->clear();
		board.draw(layout.board);
		piece.draw(layout.board);
		layout.board->refresh();
	}

	Ncurses::exit();
	return 0;
}


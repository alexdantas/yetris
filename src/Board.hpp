#ifndef BOARD_H_DEFINED
#define BOARD_H_DEFINED

#include <vector>
#include "Block.hpp"
#include "Window.hpp"
#include "Piece.hpp"

#define DEFAULT_BOARD_WIDTH  10
#define DEFAULT_BOARD_HEIGHT 20

///
class Board
{
public:
	Board(int x, int y, int width, int height);
	void draw(Window* win);

	void lockPiece(Piece* piece);
	bool isPieceValid(Piece* p);

private:
	int x;
	int y;
	int width;
	int height;

	std::vector< std::vector <Block*> > block;
};

#endif //BOARD_H_DEFINED


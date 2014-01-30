#ifndef PIECE_H_DEFINED
#define PIECE_H_DEFINED

#include "Block.hpp"

///
class Piece
{
public:
	enum PieceType
	{
		PIECE_O = 0,
		PIECE_I,
		PIECE_L,
		PIECE_J,
		PIECE_Z,
		PIECE_S,
		PIECE_T,
		PIECE_MAX,
		PIECE_DUMMY
	};
	enum PieceDirection
	{
		DIR_NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
	};

	Piece(PieceType type, int x, int y);

	void draw(Window* win);

private:
	PieceType type;
	Block* block;

	int x;
	int y;

	short rotation;
};

#endif //PIECE_H_DEFINED


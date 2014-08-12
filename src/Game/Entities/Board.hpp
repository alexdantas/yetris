#ifndef BOARD_H_DEFINED
#define BOARD_H_DEFINED

#include <vector>

#include <Game/Entities/Block.hpp>
#include <Game/Entities/Piece.hpp>
#include <Engine/Graphics/Window.hpp>

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

	/// Tells if the Blocks have reached the top.
	bool isFull();

	void pushRight();
	void pushLeft();
	void pushUp();
	void pushDown();

	void addNoise(int height);

	/// Removes all filled lines, pushing others down.
	///
	/// @return The amount of lines cleared.

	int markFullLines();
	void clearFullLines();

	/// Tells if #piece can move to #direction inside this Board.
	bool pieceCanMove(Piece* piece, Piece::PieceDirection direction);

	/// Forces #piece to move all the way down inside this Board.
	void hardDrop(Piece* piece);

	int getX();
	int getY();
	int getW();
	int getH();

	void turnInvisible(bool option);

	// HACK, do we need this?
	std::vector< std::vector <Block*> > block;

private:
	int x;
	int y;
	int width;
	int height;
};

#endif //BOARD_H_DEFINED


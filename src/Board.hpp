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
	/// @note This method pauses a little to show an
	///       animation on #win.
	/// @todo Refactor this so we shouldn't explicitly
	///       depend on making the animation inside Board.
	///
	int clearFullLines(Window* win);

private:
	int x;
	int y;
	int width;
	int height;

	std::vector< std::vector <Block*> > block;
};

#endif //BOARD_H_DEFINED


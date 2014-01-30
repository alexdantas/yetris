#include "Board.hpp"
#include "PieceDefinitions.hpp"
#include "Globals.hpp"

Board::Board(int x, int y, int width, int height):
	x(x),
	y(y)
{
	this->block.resize(width);

	for (unsigned int i = 0; i < (this->block.size()); i++)
		this->block[i].resize(height);

	this->width  = this->block.size();
	this->height = this->block[0].size();

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			this->block[i][j] = NULL;
}
void Board::draw(Window* win)
{
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			if (this->block[i][j])
				this->block[i][j]->draw(win,
				                        (this->x + i) * 2, // 2 chars
				                        this->y + j);
}
void Board::lockPiece(Piece* p)
{
	// Looking up on the global PieceDefinitions table
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (global_pieces[p->getType()][p->getRotation()][j][i] != 0)
			{
				int block_x = p->getX() + i;
				int block_y = p->getY() + j;

				this->block[block_x][block_y] = p->getBlock();

				if (Globals::Theme::lock_piece_color)
					this->block[block_x][block_y] = Globals::Theme::locked;
			}
}
bool Board::isPieceValid(Piece* p)
{
	// Looking up on the global PieceDefinitions table
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (global_pieces[p->getType()][p->getRotation()][j][i] != 0)
			{
				int block_x = p->getX() + i;
				int block_y = p->getY() + j;

				if ((block_x >= this->x + this->width) ||
				    (block_x <  this->x) ||
				    (block_y >= this->y + this->height))
					return false;

				// If this block is still out of the board
				// (from above)
				// we don't check collision
				if (block_y < 0)
					continue;

				// Fellow blocks check
				if (this->block[block_x][block_y])
					return false;
			}

	return true;
}



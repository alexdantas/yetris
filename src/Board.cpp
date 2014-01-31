#include "Board.hpp"
#include "PieceDefinitions.hpp"
#include "Globals.hpp"
#include "Utils.hpp"
#include <unistd.h>

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

				if (block_x < 0 || block_y < 0)
					continue;

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
bool Board::isFull()
{
	// Watching the top line for any visible blocks
	for (int i = 0; i < (this->width); i++)
		if (this->block[i][0])
			return true;

	return false;
}
void Board::pushLeft()
{
	for (int j = 0; j < (this->height); j++)
	{
		Block* tmp = this->block[0][j];

		for (int i = 0; i < (this->width - 1); i++)
			this->block[i][j] = this->block[i + 1][j];

		this->block[(this->width) - 1][j] = tmp;
	}
}
void Board::pushRight()
{
	for (int j = 0; j < this->height; j++)
	{
		Block* tmp = this->block[this->width - 1][j];

		for (int i = (this->width - 1); i > 0; i--)
			this->block[i][j] = this->block[i - 1][j];

		this->block[0][j] = tmp;
	}
}
void Board::pushUp()
{
	for (int j = 0; j < (this->height - 1); j++)
		for (int i = 0; i < (this->width); i++)
			this->block[i][j] = this->block[i][j + 1];

	for (int i = 0; i < (this->width); i++)
		if (Utils::Random::boolean())
			this->block[i][this->height - 1] = Globals::Theme::piece_colorless;
}
void Board::pushDown()
{
	for (int j = (this->height - 1); j > 1; j--)
		for (int i = 0; i < (this->width); i++)
			this->block[i][j] = this->block[i][j - 1];

	for (int i = 0; i < this->width; i++)
		this->block[i][0] = NULL;
}
void Board::addNoise(int height)
{
	if (height > (this->height - 1))
		height = (this->height - 1);

	for (int i = 0; i < height; i++)
		this->pushUp();
}
int Board::clearFullLines(Window* win)
{
	int lines_cleared = 0;

	// Loop through all lines,
	// doing something only on those marked 'true'

	for (int k = 0; k < (this->height); k++)
	{
		// Checking if we must delete the line
		// (completely full)
		bool skip = false;

		for (int i = 0; i < (this->width); i++)
		{
			if (! this->block[i][k])
			{
				skip = true;
				break;
			}
		}

		if (skip)
			continue;

		lines_cleared++;

		// Will show a nice animation for cleared lines,
		// changing the appearance of the line just before
		// cleared and waiting for a little bit

		for (int i = 0; i < (this->width); i++)
			this->block[i][k] = Globals::Theme::clear_line;
	}

	if (lines_cleared == 0)
		return 0;

	// Redrawing the board and sleeping for a few moments
	this->draw(win);
	win->refresh();
	usleep(Globals::Game::line_clear_timeout_us);

	for (int k = 0; k < (this->height); k++)
	{
		if (! (this->block[0][k] == Globals::Theme::clear_line))
			continue;

		// Moves all upper lines one row down,
		// starting from the second down
		// (meaning the first line is always NULL)
		for (int j = k; j > 0; j--)
			for (int i = 0; i < (this->width); i++)
				this->block[i][j] = this->block[i][j - 1];
	}
	return lines_cleared;
}
bool Board::pieceCanMove(Piece* piece, Piece::PieceDirection direction)
{
	Piece tmp = (*piece);

	tmp.move(direction);
	return this->isPieceValid(&tmp);
}
void Board::hardDrop(Piece* piece)
{
	while (this->pieceCanMove(piece, Piece::DIR_DOWN))
		piece->move(Piece::DIR_DOWN);
}
int Board::getX()
{
	return this->x;
}
int Board::getY()
{
	return this->y;
}
int Board::getW()
{
	return this->width;
}
int Board::getH()
{
	return this->height;
}


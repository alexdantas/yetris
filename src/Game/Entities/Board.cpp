#include <Game/Entities/Profile.hpp>
#include <Game/Entities/Board.hpp>
#include <Game/Entities/PieceDefinitions.hpp>
#include <Game/Config/Globals.hpp>
#include <Engine/Helpers/Utils.hpp>

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

				if (Globals::Profiles::current->settings.theme.lock_piece_color)
					this->block[block_x][block_y] = Globals::Profiles::current->settings.theme.locked;
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
	// Pushing everything up except for the bottom line
	for (int j = 0; j < (this->height - 1); j++)
		for (int i = 0; i < (this->width); i++)
			this->block[i][j] = this->block[i][j + 1];

	// Adding random blocks on the bottom line.
	//
	// We have to assure there's at least a blank block
	// or else it will get cleared.
	int blank = Utils::Random::between(0, (this->width - 1));

	for (int i = 0; i < (this->width); i++)
	{
		if (i == blank)
		{
			this->block[i][this->height - 1] = NULL;
			continue;
		}

		if (Utils::Random::boolean())
			this->block[i][this->height - 1] = Globals::Profiles::current->settings.theme.piece_colorless;
	}
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
int Board::markFullLines()
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
			this->block[i][k] = Globals::Profiles::current->settings.theme.clear_line;
	}

	return lines_cleared;
}
void Board::clearFullLines()
{
	for (int k = 0; k < (this->height); k++)
	{
		if (! (this->block[0][k] == Globals::Profiles::current->settings.theme.clear_line))
			continue;

		// Moves all upper lines one row down,
		// starting from the second down
		// (meaning the first line is always NULL)
		for (int j = k; j > 0; j--)
			for (int i = 0; i < (this->width); i++)
				this->block[i][j] = this->block[i][j - 1];
	}
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
void Board::turnInvisible(bool option)
{
	for (int i = 0; i < (this->width); i++)
		for (int j = 0; j < (this->height); j++)

			if (option)
			{
				if (this->block[i][j])
					this->block[i][j] = Globals::Profiles::current->settings.theme.invisible;
			}
			else
			{
				if (this->block[i][j] == Globals::Profiles::current->settings.theme.invisible)
					this->block[i][j] = Globals::Profiles::current->settings.theme.piece;
			}
}


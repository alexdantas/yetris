#include "Piece.hpp"
#include "Globals.hpp"
#include "Window.hpp"
#include "PieceDefinitions.hpp"
#include "Utils.hpp"

// Local function that converts between an `int` and my `enum`.
Piece::PieceType intToPieceType(int num)
{
	switch(num)
	{
	case 0: return Piece::PIECE_O;
	case 1: return Piece::PIECE_I;
	case 2: return Piece::PIECE_L;
	case 3: return Piece::PIECE_J;
	case 4: return Piece::PIECE_Z;
	case 5: return Piece::PIECE_S;
	case 6: return Piece::PIECE_T;
	case 7: return Piece::PIECE_MAX;
	case 8: return Piece::PIECE_DUMMY;
	}
	return Piece::PIECE_DUMMY;
}

Piece::PieceType Piece::random()
{
	// Default 'smart' algorithm
	//
	// The logic is to keep a bag with
	// one of each piece (7 pieces total).
	//
	// Then we take one after another in a random order.
	// As soon as we took them all,
	// place them back on the bag and redo it.
	//
	// This way, we avoid long sequences
	// of the same piece and guarantee a certain
	// degree of piece rotativity ("I WANT TEH LINES!11!!").
	//
	if (Globals::Game::random_algorithm == 1)
	{
		int i,j;
		int size = PIECE_MAX;

		static Piece::PieceType bag[PIECE_MAX];
		static int piece = PIECE_MAX;

		if (piece >= PIECE_MAX)
		{
			piece = 0;

			bag[0] = intToPieceType(Utils::Random::between(0, size - 1));
			for (i = 1; i < size; i++)
			{
				bag[i] = intToPieceType(Utils::Random::between(0, size - 1));

				j = 0;
				do {				///for (j = 0; j < i; j++)

					if (bag[j] == bag[i])
					{
						bag[i] = intToPieceType(Utils::Random::between(0, size - 1));
						j = 0;
					}
					else
						j++;

				} while (j < i);
			}
		}
		piece++;
		return bag[piece - 1];
	}
	// else if (Globals::Game::random_algorithm == 2)
	// {
	// 	// Dummy random algorithm
	// }
	else
	{
		// Simple srand() algorithm
		return intToPieceType(Utils::Random::between(0, (PIECE_MAX - 1)));
	}
}

Piece::Piece(Piece::PieceType type, int x, int y):
	type(type),
	x(x),
	y(y),
	rotation(0)
{
	switch(type)
	{
	case PIECE_S: this->block = Globals::Theme::piece_S; break;
	case PIECE_Z: this->block = Globals::Theme::piece_Z; break;
	case PIECE_O: this->block = Globals::Theme::piece_O; break;
	case PIECE_I: this->block = Globals::Theme::piece_I; break;
	case PIECE_L: this->block = Globals::Theme::piece_L; break;
	case PIECE_J: this->block = Globals::Theme::piece_J; break;
	case PIECE_T: this->block = Globals::Theme::piece_T; break;

	default:
		this->block = Globals::Theme::piece_colorless;
		break;
	}

	/* The starting positions are derived from the SRS */
	// p.x = BOARD_WIDTH/2 + global_pieces_position[p.type][p.rotation][0];
	// p.y = global_pieces_position[p.type][p.rotation][1];

}
void Piece::draw(Window* win)
{
	if (!this->block)
		return;

	// Looking up the global table of
	// possible piece positions and rotations
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (global_pieces[this->type][this->rotation][j][i] != 0)
				this->block->draw(win,
				                  (this->x + i) * 2,
				                  this->y + j);
}
void Piece::rotate(short rotation)
{
	// Rotating to the negative side (counter-clockwise)
	if (rotation < 0)
		rotation += 4;

	// keep it under 4
	this->rotation = (this->rotation + rotation) % 4;
}
void Piece::move(Piece::PieceDirection direction)
{
	int delta_x = 0;
	int delta_y = 0;

	switch (direction)
	{
	case DIR_LEFT:  delta_x = -1; break;
	case DIR_RIGHT: delta_x =  1; break;
	case DIR_DOWN:  delta_y =  1; break;
	case DIR_UP:    delta_y = -1; break;
	default: return;
	}

	this->x += delta_x;
	this->y += delta_y;
}
void Piece::move(int dx, int dy)
{
	this->x += dx;
	this->y -= dy;
}
int Piece::getX()
{
	return this->x;
}
int Piece::getY()
{
	return this->y;
}
Block* Piece::getBlock()
{
	return this->block;
}
Piece::PieceType Piece::getType()
{
	return this->type;
}
short Piece::getRotation()
{
	return this->rotation;
}


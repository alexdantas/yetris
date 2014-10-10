#include <Game/Entities/Piece.hpp>
#include <Game/Entities/PieceDefinitions.hpp>
#include <Game/Config/Globals.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Graphics/Window.hpp>
#include <Game/Entities/Profile.hpp>

// Local function that converts between an `int` and my `enum`.
Piece::PieceType intToPieceType(int num)
{
	switch(num)
	{
	case 0: return Piece::O;
	case 1: return Piece::I;
	case 2: return Piece::L;
	case 3: return Piece::J;
	case 4: return Piece::Z;
	case 5: return Piece::S;
	case 6: return Piece::T;
	case 7: return Piece::MAX;
	}
	return Piece::MAX;
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
	if (Globals::Profiles::current->scores->score.random_algorithm == "regular")
	{
		int i,j;
		int size = Piece::MAX;

		static Piece::PieceType bag[Piece::MAX];
		static int piece = Piece::MAX;

		if (piece >= Piece::MAX)
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
	else
	{
		// Dumb srand() algorithm
		return intToPieceType(Utils::Random::between(0, (Piece::MAX - 1)));
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
	case Piece::S: this->block = Globals::Profiles::current->settings.theme.piece_S; break;
	case Piece::Z: this->block = Globals::Profiles::current->settings.theme.piece_Z; break;
	case Piece::O: this->block = Globals::Profiles::current->settings.theme.piece_O; break;
	case Piece::I: this->block = Globals::Profiles::current->settings.theme.piece_I; break;
	case Piece::L: this->block = Globals::Profiles::current->settings.theme.piece_L; break;
	case Piece::J: this->block = Globals::Profiles::current->settings.theme.piece_J; break;
	case Piece::T: this->block = Globals::Profiles::current->settings.theme.piece_T; break;

	default:
		this->block = Globals::Profiles::current->settings.theme.piece_colorless;
		break;
	}
}
void Piece::draw(Window* win)
{
	if (!this->block)
		return;

	if (this->type >= Piece::MAX)
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
void Piece::moveBy(int dx, int dy)
{
	this->x += dx;
	this->y -= dy;
}
void Piece::moveTo(int x, int y)
{
	this->x = x;
	this->y = y;
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


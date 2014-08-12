#include <Game/Entities/RotationSystemSRS.hpp>

char srs_possible_positions[2][2][4][5][2] =
{
	// J, L, S, T, Z
	{
		// clockwise
		{
			// 0 -> R
			{
				{ 0,  0},
				{-1,  0},
				{-1, -1},
				{ 0,  2},
				{-1,  2},
			},
			// R -> 2
			{
				{ 0,  0},
				{ 1,  0},
				{ 1,  1},
				{ 0, -2},
				{ 1, -2}
			},
			// 2 -> L
			{
				{ 0,  0},
				{ 1,  0},
				{ 1, -1},
				{ 0,  2},
				{ 1,  2}
			},
			// L -> 0
			{
				{ 0,  0},
				{-1,  0},
				{-1,  1},
				{ 0, -2},
				{-1, -2}
			}
		},
		// counter-clockwise
		{
			// 0 -> L
			{
				{ 0,  0},
				{ 1,  0},
				{ 1, -1},
				{ 0,  2},
				{ 1,  2}
			},
			// L -> 2
			{
				{ 0,  0},
				{-1,  0},
				{-1,  1},
				{ 0, -2},
				{-1, -2}
			},
			// 2 -> R
			{
				{ 0,  0},
				{-1,  0},
				{-1, -1},
				{ 0,  2},
				{-1,  2}
			},
			// R -> 0
			{
				{ 0,  0},
				{ 1,  0},
				{ 1,  1},
				{ 0, -2},
				{ 1, -2}
			}
		}
	},
	// I
	{
		// clockwise
		{
			// 0 -> R
			{
				{-1,  0},
				{ 1,  0},
				{-2,  0},
				{ 1, -1},
				{-2,  2}
			},
			// R -> 2
			{
				{-1,  0},
				{ 1, -1},
				{-2, -1},
				{ 1,  1},
				{-2,  2}
//				{ 2,  0}

			},
			// 2 -> L
			{
				{ 1,  0},
				{-1,  0},
				{-2,  0},
				{-1,  1},
				{-2, -2}
			},
			// L -> 0
			{
				{ 0,  1},
				{-1,  1},
				{ 2,  1},
				{-1, -1},
				{ 2,  2}
			}
		},
		// counter-clockwise
		{
			// 0 -> L
			{
				{ 0, -1},
				{ 1, -1},
				{-2, -1},
				{ 1,  1},
				{-2, -2}
			},
			// L -> 2
			{
				{-1,  0},
				{ 1,  0},
				{-2,  0},
				{ 1, -1},
				{-2,  2}
			},
			// 2 -> R
			{
				{ 0,  1},
				{-1, -1},
				{-2,  1},
				{-1, -1},
				{ 2,  2}
			},
			// R -> 0
			{
				{ 1,  1},
				{-1,  1},
				{ 2,  2},
				{-2,  2},
				{ 2, -2}
			}
		}
	}
	// O doesnt spin
};

// This need to be here otherwise
// gives "undefined reference for vtable" g++ error.
RotationSystemSRS::~RotationSystemSRS()
{ }

void RotationSystemSRS::rotate(Piece* piece, Board* board, short rotation)
{
	Piece tmp = (*piece);

	// Here's the simplest case - the piece rotates just fine
	tmp.rotate(rotation);
	if (board->isPieceValid(&tmp))
	{
		piece->rotate(rotation);
		return;
	}

	// The SRS depends on several informations about the piece.
	// First, it's type
	short srs_type;
	int rotation_num;

	if (piece->getType() == Piece::I)
	{
		srs_type = 0;
		rotation_num = piece->getRotation();
	}
	else if (piece->getType() == Piece::O)
	{
		return;
	}
	else
	{
		srs_type = 0;
		rotation_num = tmp.getRotation();
	}

	// Then if the player wants to rotate it
	// clockwise or counter-clockwise

	int rot_way;
	if (rotation > 0)
		rot_way = 0;
	else
		rot_way = 1;

	// This will make 5 tests, trying to move the piece around.
	// If any of them pass, the piece is immediately moved.
	// If none of them pass, well...
	// Lets say we're out of luck.

	for (int i = 0; i < 5; i++)
	{
		short dx = srs_possible_positions[srs_type][rot_way][rotation_num][i][0];
		short dy = srs_possible_positions[srs_type][rot_way][rotation_num][i][1];

		// Remember, the piece #tmp has already been rotated.
		// Here we just try to move it,
		// making it perform wall/floor kicks
		tmp.moveBy(dx, dy);

		// alright, if it worked for #tmp, it will work for #p
		if (board->isPieceValid(&tmp))
		{
			piece->rotate(rotation);
			piece->moveBy(dx, dy);
			return;
		}

		// Well, it didnt work, so lets return #tmp
		// to its initial position
		tmp.moveBy(-dx, -dy);
	}
}


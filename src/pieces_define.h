/* yetris - Tetris(tm) on the console.
 * Copyright (C) 2013 Alexandre Dantas (kure)
 *
 * This file is part of yetris.
 *
 * yetris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://www.github.com/alexdantas/yetris/
 * mailto:   alex.dantas92@gmail.com
 */

#ifndef PIECES_DEFINITIONS_H_DEFINED
#define PIECES_DEFINITIONS_H_DEFINED

/** This defines all pieces and possible rotations.
 *  We have 7 pieces, 4 rotations for each. To every rotation, there's
 *  a 5x5 char array
 *
 *  0 = no block
 *  1 = normal block
 *  2 = pivot block (center of rotation)
 */
char global_pieces[7][4][5][5] =
{
	/* O (square) */
	{
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 2, 1, 0},
			{0, 0, 1, 1, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 2, 1, 0},
			{0, 0, 1, 1, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 2, 1, 0},
			{0, 0, 1, 1, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 2, 1, 0},
			{0, 0, 1, 1, 0},
			{0, 0, 0, 0, 0}
		}
	},
	/* I */
	{
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 1, 2, 1, 1},
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 2, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 1, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{1, 1, 2, 1, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 1, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 2, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0}
		}
	},
	/* L */
	{
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 2, 0, 0},
			{0, 0, 1, 1, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 1, 2, 1, 0},
			{0, 1, 0, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 1, 1, 0, 0},
			{0, 0, 2, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 1, 0},
			{0, 1, 2, 1, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0}
		}
	},
	/* J */
	{
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 2, 0, 0},
			{0, 1, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 1, 0, 0, 0},
			{0, 1, 2, 1, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 1, 0},
			{0, 0, 2, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 1, 2, 1, 0},
			{0, 0, 0, 1, 0},
			{0, 0, 0, 0, 0}
		}
	},
	/* S */
	{
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 1, 0},
			{0, 0, 2, 1, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 1, 2, 0, 0},
			{0, 0, 1, 1, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 1, 2, 0, 0},
			{0, 1, 0, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 1, 1, 0, 0},
			{0, 0, 2, 1, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0}
		}
	},
	/* Z */
	{
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 2, 1, 0},
			{0, 0, 0, 1, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 2, 1, 0},
			{0, 1, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 1, 0, 0, 0},
			{0, 1, 2, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 1, 0},
			{0, 1, 2, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0}
		}
	},
	/* T */
	{
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 2, 1, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 1, 2, 1, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 1, 2, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 1, 2, 1, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0}
		}
	}
};

/** The initial position of every piece (how they'll show up on top)
 *
 *  We have 7 pieces, each have 4 rotations and each have a x anf y offset
 *
 */
int global_pieces_position[7][4][2] =
{
	/* O */
	{
		{-2, -3},
		{-2, -3},
		{-2, -3},
		{-2, -3}
	},
	/* I */
	{
		{-2, -2},
		{-2, -3},
		{-2, -2},
		{-2, -3}
	},
	/* L */
	{
		{-2, -3},
		{-2, -3},
		{-2, -3},
		{-2, -2}
	},
	/* J */
	{
		{-2, -3},
		{-2, -2},
		{-2, -3},
		{-2, -3}
	},
	/* S */
	{
		{-2, -3},
		{-2, -3},
		{-2, -3},
		{-2, -2}
	},
	/* Z */
	{
		{-2, -3},
		{-2, -3},
		{-2, -3},
		{-2, -2}
	},
	/* T */
	{
		{-2, -3},
		{-2, -3},
		{-2, -3},
		{-2, -2}
	}
};

#endif /* PIECES_DEFINITIONS_H_DEFINED */


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
 * homepage: http://yetris.alexdantas.net
 * mailto:   eu@alexdantas.net
 */

#ifndef PIECE_H_DEFINED
#define PIECE_H_DEFINED

#include "block.h"

/* How many blocks exists around the piece. Check the global
 * array #pieces -- each piece has 5 x 5 blocks */
#define PIECE_BLOCKS 5

/** Enumerate all possible pieces.
 *  @note This order is important.
 *  Also, PIECE_MAX tells the biggest number for a visible piece.
 *  The others are flags for special pieces.
 */
typedef enum { PIECE_O = 0, PIECE_I, PIECE_L,
               PIECE_J,     PIECE_Z, PIECE_S, PIECE_T,
               PIECE_MAX,   PIECE_DUMMY } piece_type_e;

/** Possible directions a piece can move */
typedef enum { DIR_NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } direction_e;

/* Forward declaration of the board type -- check #board.h */
#ifndef BOARD_S_DEFINED
#define BOARD_S_DEFINED
typedef struct board_s board_s;
#endif

struct piece_s
{
	piece_type_e type;

	int     x;
	int     y;
	block_s block[4]; /* all the piece's blocks */
	short   rotation; /* current rotation number (0 is none) */
};
#ifndef PIECE_S_DEFINED
#define PIECE_S_DEFINED
typedef struct piece_s piece_s;
#endif

piece_s new_piece(piece_type_e type);
bool piece_rotate_if_possible(piece_s* p, board_s* b, int rotation);
void piece_rotate(piece_s* p, int rotation);
bool piece_move_if_possible(piece_s* p, board_s* b, direction_e dir);
void piece_move(piece_s* p, direction_e dir);
void piece_hard_drop(piece_s* p, board_s* b);
piece_type_e piece_get_random();
bool piece_can_move(piece_s* p, board_s* b, direction_e dir);
bool piece_is_valid(piece_s* p);
bool piece_is_on_valid_position(piece_s* p, board_s* b);

#endif /* PIECE_H_DEFINED */


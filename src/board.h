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

#ifndef BOARD_H_DEFINED
#define BOARD_H_DEFINED

#include <stdbool.h>
#include "block.h"

/** The total width of the board (in blocks) */
#define BOARD_WIDTH  10

/** The total height of the board (in blocks) */
#define BOARD_HEIGHT 20

/* Forward declaration of the piece type -- see #piece.h */
#ifndef PIECE_S_DEFINED
#define PIECE_S_DEFINED
typedef struct piece_s piece_s;
#endif


/** Contains all blocks on the current game */
struct board_s
{
	block_s block[BOARD_WIDTH][BOARD_HEIGHT];

};

#ifndef BOARD_S_DEFINED
#define BOARD_S_DEFINED
typedef struct board_s board_s;
#endif


board_s new_board();
void board_lock_piece(board_s* b, piece_s* p);
void board_delete_line(board_s* b, int line);
bool board_is_full(board_s* b);
void board_delete_lines(board_s* b, bool lines[]);

#endif /* BOARD_H_DEFINED */

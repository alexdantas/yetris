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
#include "globals.h"

/** The total width of the board (in blocks) */
#define BOARD_WIDTH  10

/** The total height of the board (in blocks) */
#define BOARD_HEIGHT 20

/* Forward declaration of the piece type -- see #piece.h */
#ifndef PIECE_S_DEFINED
#define PIECE_S_DEFINED
typedef struct piece_s piece_s;
#endif


/**
 * The great container, where the blocks go rest.
 *
 * Note that we don't have any actual blocks in here.
 * What we have are pointers to appearances.
 *
 */
struct board_s
{
	int x;
	int y;

	block_theme_s* block[BOARD_WIDTH][BOARD_HEIGHT];

};

#ifndef BOARD_S_DEFINED
#define BOARD_S_DEFINED
typedef struct board_s board_s;
#endif

/**
 * Returns a brand new board.
 */
board_s new_board(int x, int y);

/**
 * Locks all the blocks from piece #p on the board #b.
 */
void board_lock_piece(board_s* b, piece_s* p);

/**
 * Deletes all the lines specified by the array #lines.
 *
 * @note This function's very ugly... FIX IT LATER.
 */
void board_delete_lines(board_s* b, bool lines[]);

/**
 * Tells if #b is filled (pieces stacked at the top).
 */
bool board_is_full(board_s* b);

/**
 * Moves all blocks inside the board one column left,
 * overlapping the first to the last.
 *
 * Just for fun.
 */
void board_slide_left(board_s* b);

/**
 * Same as board_slide_left(), only right.
 */
void board_slide_right(board_s* b);

/**
 * Adds random blocks on #b up to #height rows.
 */
void board_add_noise(board_s* b, int height);

/**
 * Moves all blocks on #b one row up,
 * adding noise on the bottom row.
 */
void board_bump_up(board_s* b);

#endif /* BOARD_H_DEFINED */


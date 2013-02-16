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

#include <stdbool.h>
#include <unistd.h> /* usleep */
#include "board.h"
#include "block.h"
#include "piece.h"

board_s new_board()
{
	board_s b;
	int i, j;

	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			b.block[i][j].type = EMPTY;

	return b;
}

/** Locks all the blocks from piece #p on the board #b */
void board_lock_piece(board_s* b, piece_s* p)
{
	int k;
	for (k = 0; k < 4; k++)
	{
		int block_x = p->block[k].x;
		int block_y = p->block[k].y;
		b->block[block_x][block_y] = p->block[k];
	}
}

/** Deletes all the lines specified by the array #lines.
 *
 *  @note I know this function's ugly... I'll fix that later
 */
void board_delete_lines(board_s* b, bool lines[])
{
	int i, j, k;
	char clear1, clear2;

	/* The appearance of the line just before it's deleted */
	if (global.theme_clear_line[0] != '\0')
	{
		clear1 = global.theme_clear_line[0];
		clear2 = global.theme_clear_line[1];
	}
	else
	{
		clear1 = ':';
		clear2 = ':';
	}

	/* loop through all lines, doing something only on those marked 'true' */
	for (k = 0; k < BOARD_HEIGHT; k++)
	{
		if (!lines[k]) continue;

		/* A nice little animation for destroyed lines */
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			b->block[i][k].theme[0] = clear1;
			b->block[i][k].theme[1] = clear2;
			b->block[i][k].color    = engine_get_color(WHITE_BLACK, true);
		}
	}

	engine_draw_board(b);
	wrefresh(engine.screen.board.win);
	usleep(200 * 1000);

	for (k = 0; k < BOARD_HEIGHT; k++)
	{
		if (!lines[k]) continue;

		/* Moves all upper lines one row down */
		for (j = k; j > 0; j--)
		{
			for (i = 0; i < BOARD_WIDTH; i++)
			{
				b->block[i][j] = b->block[i][j - 1];
				b->block[i][j].y += 1;
			}
		}
	}
}

bool board_is_full(board_s* b)
{
	/* Currently, we watch the first line for any non-EMPTY blocks */
	int i;
	for (i = 0; i < BOARD_WIDTH; i++)
		if (b->block[i][0].type != EMPTY)
			return true;

	return false;
}


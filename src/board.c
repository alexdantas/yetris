
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

	/* loop through all lines, doing something only on those marked 'true' */
	for (k = 0; k < BOARD_HEIGHT; k++)
	{
		if (!lines[k]) continue;

		/* A nice little animation for destroyed lines */
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			b->block[i][k].theme[0] = ':';
			b->block[i][k].theme[1] = ':';
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


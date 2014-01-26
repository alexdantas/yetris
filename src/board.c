
#include <stdbool.h>
#include <unistd.h> /* usleep */
#include "board.h"
#include "piece.h"
#include "piece_definitions.h"
#include "engine.h"
#include "utils.h"

board_s new_board(int x, int y)
{
	board_s b;

	b.x = x;
	b.y = y;

	/* Clear everything */
	int i, j;
	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			b.block[i][j] = NULL;

	return b;
}

void board_lock_piece(board_s* b, piece_s* p)
{
	/* We're simply looking up on the global "table"
	 * of piece positions and rotations.
	 */
	int i, j;

	for (i = 0; i < PIECE_BLOCKS; i++)
	{
		for (j = 0; j < PIECE_BLOCKS; j++)
		{
			if (global_pieces[p->type][p->rotation][j][i] != 0)
			{
				int block_x = i + p->x;
				int block_y = j + p->y;

				b->block[block_x][block_y] = p->theme;

				if (global.theme_lock_piece_color)
					b->block[block_x][block_y] = &(global.theme_locked);
			}
		}
	}
}

void board_delete_lines(board_s* b, bool lines[])
{
	int i, j, k;

	/* loop through all lines,
	 * doing something only on those marked 'true' */

	for (k = 0; k < BOARD_HEIGHT; k++)
	{
		if (! lines[k]) continue;

		/* A nice little animation for destroyed lines */
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			/* The appearance of the line just before it's deleted */
			b->block[i][k] = &(global.theme_clear_line);

			// BUG! If I set bold to 'true' all pieces become bold!
		}
	}

	/* Redrawing the board - perhaps I should do it other way? */
	engine_draw_board(b);
	wrefresh(engine.screen.board.win);

	/* Sleeping for a few moments */
	usleep(global.game_line_clear_timeout);

	for (k = 0; k < BOARD_HEIGHT; k++)
	{
		if (!lines[k]) continue;

		/* Moves all upper lines one row down,
		 * starting from the second down
		 * (meaning the first line is always NULL)
		 */
		for (j = k; j > 0; j--)
			for (i = 0; i < BOARD_WIDTH; i++)
				b->block[i][j] = b->block[i][j - 1];
	}
}

bool board_is_full(board_s* b)
{
	/* Currently, we watch the first line for any visible blocks */
	int i;
	for (i = 0; i < BOARD_WIDTH; i++)
		if (b->block[i][0] != NULL)
			return true;

	return false;
}

void board_push_left(board_s* b)
{
	int i, j;

	for (j = 0; j < BOARD_HEIGHT; j++)
	{
		block_theme_s* tmp = b->block[0][j];

		for (i = 0; i < (BOARD_WIDTH - 1); i++)
			b->block[i][j] = b->block[i + 1][j];

		b->block[BOARD_WIDTH - 1][j] = tmp;
	}
}

void board_push_right(board_s* b)
{
	int i, j;

	for (j = 0; j < BOARD_HEIGHT; j++)
	{
		block_theme_s* tmp = b->block[BOARD_WIDTH - 1][j];

		for (i = (BOARD_WIDTH - 1); i > 0; i--)
			b->block[i][j] = b->block[i - 1][j];

		b->block[0][j] = tmp;
	}
}

void board_add_noise(board_s* b, int height)
{
	if (height > BOARD_HEIGHT)
		height = BOARD_HEIGHT;

	int i, j;

	for (i = 0; i < BOARD_WIDTH; i++)
	{
		for (j = BOARD_HEIGHT - height;
		     j < BOARD_HEIGHT;
		     j++)
		{
			if (random_bool())
			{
				piece_s tmp = new_piece(piece_random_type());

				b->block[i][j] = tmp.theme;
			}
		}
	}
}

void board_push_up(board_s* b)
{
	int i, j;

	for (j = 0; j < (BOARD_HEIGHT - 1); j++)
		for (i = 0; i < BOARD_WIDTH; i++)
			b->block[i][j] = b->block[i][j + 1];

	for (i = 0; i < BOARD_WIDTH; i++)
		if (random_bool())
			b->block[i][BOARD_HEIGHT - 1] = &(global.theme_piece_colorless);
}

void board_push_down(board_s* b)
{
	int i, j;

	for (j = (BOARD_HEIGHT - 1); j > 1; j--)
		for (i = 0; i < BOARD_WIDTH; i++)
			b->block[i][j] = b->block[i][j - 1];

	for (i = 0; i < BOARD_WIDTH; i++)
		b->block[i][0] = NULL;
}


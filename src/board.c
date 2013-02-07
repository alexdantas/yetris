
#include <stdbool.h>
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

/** Puts the piece #p on the board */
void board_save_piece(board_s* b, piece_s* p)
{
	int k;
	for (k = 0; k < 4; k++)
	{
		int block_x = p->block[k].x;
		int block_y = p->block[k].y;
		b->block[block_x][block_y] = p->block[k];
	}
}

/** Deletes the line specified by #line */
void board_delete_line(board_s* b, int line)
{
	int i, j;

	/* A nice little animation for destroyed lines */
	for (i = 0; i < BOARD_WIDTH; i++)
	{
		b->block[i][line].theme[0] = ':';
		b->block[i][line].theme[1] = ':';
		b->block[i][line].color    = WHITE_BLACK;
	}

	engine_draw_board(b);
	wrefresh(engine.screen.board.win);
	usleep(300 * 1000);

	/* Moves all upper lines one row down */
	for (j = line; j > 0; j--)
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			b->block[i][j] = b->block[i][j - 1];
			b->block[i][j].y += 1;
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


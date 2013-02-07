
#include <unistd.h>
#include "board.h"
#include "block.h"

board_s new_board()
{
	board_s b;
	int i, j;

	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			b.block[i][j].type = EMPTY;

	return b;
}

void engine_draw_board(board_s* b)
{
	int i, j;

	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			if (b->block[i][j].type != EMPTY)
				engine_draw_block(&(b->block[i][j]));
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
	wrefresh(engine.screen.board);
	usleep(300 * 1000);

	/* Moves all upper lines one row down */
	for (j = line; j > 0; j--)
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			b->block[i][j] = b->block[i][j - 1];
			b->block[i][j].y += 1;
		}
}

/** Checks all lines, deleting the ones that are full */
void board_delete_possible_lines(board_s* b)
{
	int j;
	for (j = 0; j < BOARD_HEIGHT; j++)
	{
		int i = 0;
		while (i < BOARD_WIDTH)
		{
			if (b->block[i][j].type == EMPTY)
				break;
			i++;
		}
		if (i == BOARD_WIDTH)
			board_delete_line(b, j);
	}
}


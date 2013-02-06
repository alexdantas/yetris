
/** The map containing all blocks */
typedef struct board_s
{
	block_s block[BOARD_WIDTH][BOARD_HEIGHT];

} board_s;

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


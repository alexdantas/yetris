
/** x and y are relative to the board */
typedef struct block_s
{
	char    block[3];
	color_e color;
	int x;
	int y;
} block_s;

block_s new_block(int x, int y, char block[], color_e color)
{
	block_s b;

	b.x = x;
	b.y = y;
	b.block[0] = block[0];
	b.block[1] = block[1];
	b.block[3] = '\0';
	b.color = color;

	return b;
}

/* wont call refresh */
void engine_draw_block(block_s* b)
{
	/* Is it really necessary to turn of attributes? */
	wattron(engine.screen.board, COLOR_PAIR(b->color));
	mvwaddstr(engine.screen.board, b->y, b->x * 2, b->block);
	wattroff(engine.screen.board, COLOR_PAIR(b->color));
}


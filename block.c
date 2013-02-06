
enum block_e { BLOCK, EMPTY };
typedef enum block_e block_e;

/** x and y are relative to the board */
typedef struct block_s
{
	int     x;
	int     y;
	block_e type;
	color_e color;
	char    theme[3];
} block_s;

block_s new_block(int x, int y, char theme[], color_e color)
{
	block_s b;

	b.x        = x;
	b.y        = y;
	b.color    = color;
	b.type     = BLOCK;
	b.theme[0] = theme[0];
	b.theme[1] = theme[1];
	b.theme[2] = '\0';

	return b;
}

/* wont call refresh */
void engine_draw_block(block_s* b)
{
	WINDOW* w = engine.screen.board;

	wattrset(w, COLOR_PAIR(b->color));
	mvwaddstr(w, b->y, (b->x * 2), b->theme);
}


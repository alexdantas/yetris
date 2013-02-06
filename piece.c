
#include "pieces_definitions.c"

/* How many blocks exists around the piece. Check the global
 * array #pieces -- each piece has 5 x 5 blocks */
#define PIECE_BLOCKS 5

/* How many blocks we have for width and height */
#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20


/** Enumerate all possible pieces.
 *  @note This order is important.
 *  Also, PIECE_MAX tells the biggest number for a piece
 */
typedef enum piece_e { PIECE_O = 0, PIECE_I, PIECE_J,
                       PIECE_L,     PIECE_S, PIECE_Z, PIECE_T,
                       PIECE_MAX} piece_e;

typedef struct piece_s
{
	piece_e type;
	int     x;
	int     y;
	color_e color;
	char    theme[3]; /* the appearance of each block of this piece */
	block_s block[4];
	int     rotation;

} piece_s;

piece_s new_piece(piece_e type)
{
	if ((type < 0) || (type >= PIECE_MAX))
		exit(0);

	piece_s p;
	color_e color;

	switch(type)
	{
	case PIECE_S: color = BLACK_GREEN;   break;
	case PIECE_Z: color = BLACK_RED;     break;
	case PIECE_O: color = BLACK_YELLOW | A_BOLD; break;
	case PIECE_I: color = BLACK_CYAN   | A_BOLD; break;
	case PIECE_L: color = BLACK_YELLOW;  break;
	case PIECE_J: color = BLACK_BLUE;    break;
	case PIECE_T: color = BLACK_MAGENTA; break;
	}

	p.rotation = 0;
	p.type     = type;
	p.color    = color;
	p.theme[0] = '['; p.theme[1] = ']'; p.theme[3] = '\0';
	p.x        = BOARD_WIDTH/2 + global_pieces_position[p.type][p.rotation][0];
	p.y        = global_pieces_position[p.type][p.rotation][1];

	/* This seems complicated, but it's just starting each
	 * block of the piece according to it's x and y on the board */
	int i,j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p.type][p.rotation][j][i] != 0)
			{
				int block_x  = p.x + i;
				int block_y  = p.y + j;

				p.block[k] = new_block(block_x, block_y, p.theme, p.color);
				k++;
			}
	return p;
}

void engine_draw_piece(piece_s* p)
{
	/* WARNING this function assumes there are no more than 4
	 * blocks for each piece on the #pieces global array! */
	int i, j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p->type][p->rotation][j][i] != 0)
			{
				engine_draw_block(&(p->block[k]));
				k++;
			}
}

/** Rotate piece #p by #rotation times. Negative number rotates backwards */
void piece_rotate(piece_s* p, int rotation)
{
	if (rotation < 0)
		rotation += 4;
	p->rotation = (p->rotation + rotation) % 4; /* keep it under 4 */

	int i,j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p->type][p->rotation][j][i] != 0)
			{
				p->block[k] = new_block(p->x + i, p->y + j, p->theme, p->color);
				k++;
			}
}

/** Drops piece by one tile */
void piece_soft_drop(piece_s* p)
{
	p->y++;

	int i,j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p->type][p->rotation][j][i] != 0)
			{
				p->block[k].y++;
				k++;
			}
}

/** Moves piece according to #direction. 0 is right and 1 is left */
void piece_move(piece_s* p, int direction)
{
	int delta = (direction == 0) ? 1 : -1;

	p->x += delta;

	int i,j, k = 0;
	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)
			if (global_pieces[p->type][p->rotation][j][i] != 0)
			{
				p->block[k].x += delta;
				k++;
			}
}

/** Drops piece as far as possible */
void piece_hard_drop(piece_s* p)
{

}

/** Returns a random piece */
piece_e piece_get_random()
{
	int min = 0;
	int max = PIECE_MAX - 1;
	return rand() % (max - min + 1) + min;
}


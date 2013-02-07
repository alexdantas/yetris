
#include "engine.h"
#include "block.h"

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

/** Copies block #a into #b */
void block_copy(block_s* a, block_s* b)
{
	b->x        = a->x;
	b->y        = a->y;
	b->color    = a->color;
	b->type     = a->type;
	b->theme[0] = a->theme[0];
	b->theme[1] = a->theme[1];
	b->theme[2] = a->theme[2];
}


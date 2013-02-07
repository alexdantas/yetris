
#ifndef BLOCK_H_DEFINED
#define BLOCK_H_DEFINED

#include "engine.h"

/** Which states can a block be? */
typedef enum { BLOCK, EMPTY } block_e;

/** x and y are relative to the board */
typedef struct block_s
{
	int     x;
	int     y;
	block_e type;
	color_e color;
	char    theme[3];
} block_s;

block_s new_block(int x, int y, char theme[], color_e color);
void block_copy(block_s* a, block_s* b);

#endif /* BLOCK_H_DEFINED */


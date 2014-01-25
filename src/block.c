
#include "engine.h"
#include "block.h"

block_s new_block(int x, int y, block_theme_s* theme)
{
	block_s b;

	b.x          = x;
	b.y          = y;
	b.is_visible = true;
	b.theme      = theme;
	return b;
}


/* yetris - Tetris(tm) on the console.
 * Copyright (C) 2013 Alexandre Dantas (kure)
 *
 * This file is part of yetris.
 *
 * yetris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://www.github.com/alexdantas/yetris/
 * mailto:   alex.dantas92@gmail.com
 */

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


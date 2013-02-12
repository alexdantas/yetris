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
	int     color;
	char    theme[3];
} block_s;

block_s new_block(int x, int y, char theme[], color_e color);
void block_copy(block_s* a, block_s* b);

#endif /* BLOCK_H_DEFINED */


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

/** Which states can a block be? Will not print when it's EMPTY */
typedef enum { BLOCK, EMPTY } block_e;

/** All info about individual block */
struct block_s
{
	int     x;        /**< x position relative to the board */
	int     y;        /**< y position relative to the board */
	block_e type;     /**< state of the block, see #block_e */
	int     color;    /**< color that will print the block */
	char    theme[2]; /**< appearance (how it will be printed onscreen) */
};

#ifndef _BLOCK_S
#define _BLOCK_S
typedef struct block_s block_s;
#endif

block_s new_block(int x, int y, char theme[], color_e color);

#endif /* BLOCK_H_DEFINED */


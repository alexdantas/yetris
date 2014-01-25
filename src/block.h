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
 * homepage: http://yetris.alexdantas.net
 * mailto:   eu@alexdantas.net
 */

#ifndef BLOCK_H_DEFINED
#define BLOCK_H_DEFINED

#include "engine.h"

/** All info about individual block */
struct block_s
{
	int     x;        /**< x position relative to the board */
	int     y;        /**< y position relative to the board */

	bool is_visible;
	block_theme_s* theme;
};

#ifndef BLOCK_S_DEFINED
#define BLOCK_S_DEFINED
typedef struct block_s block_s;
#endif

block_s new_block(int x, int y, block_theme_s* theme);

#endif /* BLOCK_H_DEFINED */


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
 * homepage: http://yetris.alexdantas.net/
 * mailto:   eu@alexdantas.net
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

	return b;
}


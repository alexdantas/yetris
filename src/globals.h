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

/* I know it's ugly to leave so many global variables >_<
 * please don't sue me */

#ifndef GLOBALS_H_DEFINED
#define GLOBALS_H_DEFINED

#include <stdbool.h>

#define NEXT_PIECES_NO 7 /* by default how many next pieces will be shown
						  * (1 on the top and 6 others) */

/** Game-related global variables */
typedef struct globals_s
{
	bool screen_center_vertically;
	bool screen_center_horizontally;
	bool screen_use_colors;
	bool game_has_ghost;
	bool game_can_hold;
	char game_next_no; /**< How many next pieces the user can see */

} globals_s;

/* global struct containing all global variables.. wow  */
globals_s global;

globals_s new_globals();

#endif /* GLOBALS_H_DEFINED */


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

/** @file globals.h
 * Definitions of global variables that will rule the game.
 *
 * I know it's ugly to leave so many global variables >_<
 *
 * HOW-TO add customization
 * 1- decide what it does on the game
 * 2- make a global in globals.h that defines it
 * 3- initialize it on globals.c -> new_globals() with default value
 * 4- put it on config.c -> config_parse()
 * 5- put it on config.c -> config_create_default() with default value
 * 6- finally, make use of it on the game
 * 7- dont forget to:
 *    put it on arguments.c -> args_handle() and print_usage()
 *    put it on the man page
 */

#ifndef GLOBALS_H_DEFINED
#define GLOBALS_H_DEFINED

#include <stdbool.h>

#define NEXT_PIECES_NO 6 /* by default how many next pieces will be shown
						  * (1 on the top and 5 others) */

/** Game-related global variables */
typedef struct globals_s
{
	bool screen_center_vertically;
	bool screen_center_horizontally;
	bool screen_use_colors;
	bool screen_fancy_borders;
	bool screen_show_outer_border;

	int  fps; /** On how many frames per second is the game running? */
	bool game_has_ghost;
	bool game_can_hold;
	char game_next_no; /**< How many next pieces the user can see */
	bool game_has_statistics;
	bool game_has_line_statistics;
	char game_random_algorithm;
	bool game_has_game_over_animation;
	int  game_line_clear_timeout; /**< Pause (microseconds) during line clear */

	bool theme_piece_has_colors;
	bool theme_ghost_has_colors;
	bool theme_show_pivot_block;
	bool theme_lock_piece_color;
	int  theme_clear_line_color;
	int  theme_piece_no_color;
	int  theme_ghost_color;
	int  theme_piece_S_color;
	int  theme_piece_Z_color;
	int  theme_piece_O_color;
	int  theme_piece_I_color;
	int  theme_piece_L_color;
	int  theme_piece_J_color;
	int  theme_piece_T_color;

	char theme_piece[2];
	char theme_ghost[2];
	char theme_clear_line[2];

	char theme_piece_S[2];
	char theme_piece_Z[2];
	char theme_piece_O[2];
	char theme_piece_I[2];
	char theme_piece_L[2];
	char theme_piece_J[2];
	char theme_piece_T[2];

	char* config_filename; /**< The user-specified configuration file */

} globals_s;

/* global struct containing all global variables.. wow  */
globals_s global;

globals_s new_globals();

#endif /* GLOBALS_H_DEFINED */


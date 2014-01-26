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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	 If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://yetris.alexdantas.net
 * mailto:	 eu@alexdantas.net
 */

#ifndef GLOBALS_H_DEFINED
#define GLOBALS_H_DEFINED

/**
 * @file globals.h
 * Definitions of global variables that will rule the game.
 *
 * I know it's ugly to leave so many global variables >_<
 *
 * HOW-TO add customization:
 *
 * 1. decide what it does on the game
 * 2. make a global in globals.h that defines it
 * 3. initialize it on globals.c -> new_globals() with default value
 * 4. put it on config.c -> config_parse()
 * 5. put it on config.c -> config_create_default() with
 *    default value
 * 6. finally, make use of it on the game
 * 7. dont forget to:
 *	  7.1. put it on arguments.c -> args_handle() and print_usage()
 *	  7.2. put it on the man page
 */

#include <stdbool.h>
#include "color.h"

/* By default how many next pieces will be shown
 * (1 on the top and 5 others) */
#define NEXT_PIECES_NO 6

/** How a block is shown on the screen.
 *  @bug This should be at `engine.h`!
 *       It's currently here due to an #include
 *       circular dependence.
 */
typedef struct block_theme_s
{
	color_pair_t color;
	char         appearance[2];

} block_theme_s;

void block_theme_set(block_theme_s* t, color_pair_t color, char one, char two);

/** Game-related global variables */
typedef struct globals_s
{
	char game_version[4];

	bool screen_center_vertically;
	bool screen_center_horizontally;
	bool screen_use_colors;
    bool screen_use_default_background;
	bool screen_fancy_borders;
	bool screen_show_outer_border;

	/** On how many frames per second is the game running? */
	int	fps;

	/** How many lines of noise we add at the bottom initially */
	int game_initial_noise;

	bool game_has_ghost;
	bool game_can_hold;

	/** How many next pieces the user can see */
	char game_next_no;

	bool game_has_statistics;
	bool game_has_line_statistics;
	char game_random_algorithm;
	bool game_has_game_over_animation;
	int	 game_line_clear_timeout; /**< Pause (microseconds) during line clear */
	bool game_slide_left;
	bool game_slide_right;

	bool theme_piece_has_colors;
	bool theme_ghost_has_colors;
	bool theme_show_pivot_block;
	bool theme_lock_piece_color;

	block_theme_s theme_clear_line;
	block_theme_s theme_piece_colorless;
	block_theme_s theme_piece;
	block_theme_s theme_ghost;
	block_theme_s theme_locked;
	block_theme_s theme_piece_S;
	block_theme_s theme_piece_Z;
	block_theme_s theme_piece_O;
	block_theme_s theme_piece_I;
	block_theme_s theme_piece_L;
	block_theme_s theme_piece_J;
	block_theme_s theme_piece_T;

	char config_filename[256]; /**< Absolute path to the configuration file */
	char hscore_filename[256]; /**< Absolute path to the high-score file */

} globals_s;

/* global struct containing all global variables.. wow	*/
globals_s global;

globals_s new_globals();

#endif /* GLOBALS_H_DEFINED */


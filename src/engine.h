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

#ifndef ENGINE_H_DEFINED
#define ENGINE_H_DEFINED

#include <stdlib.h>
#include <ncurses.h>
#include "globals.h"

#define NUMBER_OF_KEYS 9

/** This holds the game keymap. They all need to be int because of ncurses */
typedef struct input_s
{
	char keys[NUMBER_OF_KEYS];
	int  none;
	int  left;
	int  right;
	int  rotate;
	int  down;
	int  drop;
	int  rotate_backw;
	int  quit;
	int  pause;
	int  hold;
} input_s;

/** Stores information about a ncurses' window */
typedef struct window_s
{
	WINDOW* win;
	int     x;
	int     y;
	int     width;
	int     height;
} window_s;

/** Info about the console screen. Mostly ncurse' specific stuff */
typedef struct screen_s
{
	int width;
	int height;
	window_s main;
	window_s leftmost;
	window_s middle_left;
	window_s middle_right;
	window_s rightmost;
	window_s next_container;
	window_s board;
	window_s info;
	window_s next[NEXT_PIECES_NO];
	window_s hold;
	window_s leftmost_container;
	window_s score;
} screen_s;

/** Container for all info about the game engine */
typedef struct engine_s
{
	input_s  input;
	screen_s screen;

} engine_s;

/** Global engine structure */
engine_s engine;

/** Defining the game color pairs (background_foreground) -- arbitrary numbers */
typedef enum { BLACK_WHITE = 1, WHITE_BLACK,
                BLACK_CYAN,      CYAN_BLACK,
                BLACK_BLUE,      BLUE_BLACK,
                BLACK_RED,       RED_BLACK,
                BLACK_YELLOW,    YELLOW_BLACK,
                BLACK_MAGENTA,   MAGENTA_BLACK,
                BLACK_GREEN,     GREEN_BLACK,
 			    BLACK_BLACK,
                WHITE_CYAN,
                WHITE_BLUE,
                WHITE_RED,
                WHITE_YELLOW,
                WHITE_MAGENTA,
			    WHITE_GREEN,
                WHITE_WHITE} color_e;

/* Forward defining some structures to shut up the compiler
 * (they'll be linked on anyway) */
typedef struct block_s block_s;
typedef struct piece_s piece_s;
typedef struct board_s board_s;
typedef struct game_s  game_s;


int engine_screen_init(int width, int height);
int engine_windows_init();
bool engine_init();
bool block_signals();
bool restore_signals();
void engine_exit();
int engine_keymap(char keymap[]);
int engine_get_input(int delay_ms);
void engine_draw_block(block_s* b, WINDOW* w);
void engine_draw_piece(piece_s* p, WINDOW* w);
void engine_draw_board(board_s* b);
void engine_draw_hold(game_s* g);
void engine_draw_score(game_s* g);
void engine_draw(game_s* g);
int engine_get_color(color_e color, bool is_bold);
void engine_draw_help();
void engine_wait_for_keypress();
void engine_draw_gameover(game_s* g);
void engine_draw_pause();

#endif /* ENGINE_H_DEFINED */


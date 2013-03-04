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

/** My little hack to make yetris work on Windows. Watch out for
 *  parts where I use this define - it makes things ugly
 */
#define OS_IS_WINDOWS ((defined __WIN32__) && (!defined __CYGWIN__))

#include <stdlib.h>
#include <ncurses.h>
#include "globals.h"

/** Number of possible keystrokes - pause, left, right... */
#define NUMBER_OF_KEYS 10

/** Holds the game keymap.
 *  They all need to be int because of ncurses' keypad() */
typedef struct input_s
{
	char keys[NUMBER_OF_KEYS]; /**< Current keymap  */
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
	int  restart;
} input_s;

/** Stores information about a ncurses' window */
typedef struct window_s
{
	WINDOW* win; /**< ncurses' internal representation of a screen */
	short   x;
	short   y;
	short   width;
	short   height;
} window_s;

/** All screens of the game.
 *  Mostly ncurses' specific stuff */
typedef struct screen_s
{
	short width;       /**< Global window width */
	short height;      /**< Global window height */
	window_s main;     /**< Main window */
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
	window_s help_container;
	window_s help;
	window_s hscores_container;
	window_s hscores;
	window_s input_container;
	window_s input;

} screen_s;

/** Container for all info about the game engine */
typedef struct engine_s
{
	input_s  input;
	screen_s screen;

} engine_s;

/** Global engine structure */
engine_s engine;

/** Possible engine color pairs (FOREGROUND_BACKGROUND).
 *  This defines all possible combinations of color pairs on ncurses.
 *  Note that the order of definition is important, also the intial value
 *  of 1.
 *
 *  To access a specific color pair, use this formula:
 *    pair(n) = (foreground * 8) + background
 *
 *  Just like
 *    pair    = (COLOR_RED * 8) + COLOR_BLUE
 */
typedef enum
{
	BLACK_BLACK = 1, BLACK_RED,   BLACK_GREEN,   BLACK_YELLOW,   BLACK_BLUE,   BLACK_MAGENTA,   BLACK_CYAN,   BLACK_WHITE,
	RED_BLACK,       RED_RED,     RED_GREEN,     RED_YELLOW,     RED_BLUE,     RED_MAGENTA,     RED_CYAN,     RED_WHITE,
	GREEN_BLACK,     GREEN_RED,   GREEN_GREEN,   GREEN_YELLOW,   GREEN_BLUE,   GREEN_MAGENTA,   GREEN_CYAN,   GREEN_WHITE,
	YELLOW_BLACK,    YELLOW_RED,  YELLOW_GREEN,  YELLOW_YELLOW,  YELLOW_BLUE,  YELLOW_MAGENTA,  YELLOW_CYAN,  YELLOW_WHITE,
	BLUE_BLACK,      BLUE_RED,    BLUE_GREEN,    BLUE_YELLOW,    BLUE_BLUE,    BLUE_MAGENTA,    BLUE_CYAN,    BLUE_WHITE,
	MAGENTA_BLACK,   MAGENTA_RED, MAGENTA_GREEN, MAGENTA_YELLOW, MAGENTA_BLUE, MAGENTA_MAGENTA, MAGENTA_CYAN, MAGENTA_WHITE,
	CYAN_BLACK,      CYAN_RED,    CYAN_GREEN,    CYAN_YELLOW,    CYAN_BLUE,    CYAN_MAGENTA,    CYAN_CYAN,    CYAN_WHITE,
	WHITE_BLACK,     WHITE_RED,   WHITE_GREEN,   WHITE_YELLOW,   WHITE_BLUE,   WHITE_MAGENTA,   WHITE_CYAN,  WHITE_WHITE
} color_e;


/* Forward defining some structures to shut up the compiler
 * (they'll be linked on anyway) */
#ifndef _BLOCK_S
#define _BLOCK_S
typedef struct block_s block_s;
#endif
#ifndef _PIECE_S
#define _PIECE_S
typedef struct piece_s piece_s;
#endif
#ifndef _BOARD_S
#define _BOARD_S
typedef struct board_s board_s;
#endif
#ifndef _GAME_S
#define _GAME_S
typedef struct game_s  game_s;
#endif


int  engine_screen_init(int width, int height);
int  engine_windows_init();
bool engine_init();
bool block_signals();
bool restore_signals();
void engine_exit();
int  engine_keymap(char keymap[]);
int  engine_get_input(int delay_ms);
void engine_draw_block(block_s* b, WINDOW* w);
void engine_draw_piece(piece_s* p, WINDOW* w);
void engine_draw_board(board_s* b);
void engine_draw_hold(game_s* g);
void engine_draw_score(game_s* g);
void engine_draw(game_s* g);
int  engine_get_color(short foreground, short background, bool is_bold);
void engine_draw_help();
void engine_draw_gameover();
void engine_draw_pause();
void engine_draw_statistics(game_s* g);
void engine_safe_exit(int sig);
void engine_create_help();
void engine_delete_help();
void engine_draw_line_statistics(game_s* g);
void engine_draw_gameover_animation(game_s* g);
void engine_refresh_all_windows();

void engine_create_hscores_window();
void engine_draw_hscores();
void engine_delete_hscores_window();


void engine_create_input();
void engine_draw_input();
void engine_get_hscore_name(char* name, int size);
void engine_delete_input();

#endif /* ENGINE_H_DEFINED */


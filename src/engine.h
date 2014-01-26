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

#ifndef ENGINE_H_DEFINED
#define ENGINE_H_DEFINED

/**
 * @file engine.c
 * Implementations of graphical stuff.
 *
 * This module deals with ncurses and how to print the game
 * onscreen.
 *
 * On Micro$oft Windows systems, I use PDCurses, which has
 * less functionalities than ncurses. So I need to make a
 * few #defines to fix this.
 */

/** My little hack to make yetris work on Windows. Watch out for
 *  parts where I use this define - it makes things ugly
 */
#define OS_IS_WINDOWS ((defined __WIN32__) && (!defined __CYGWIN__))

#include <stdlib.h>
#include <ncurses.h>
#include "globals.h"
#include "color.h"
#include "layout.h"

/** Number of possible keystrokes - pause, left, right... */
#define NUMBER_OF_KEYS 10

/**
 * Holds the game keymap.
 *
 * They all need to be int because of ncurses' keypad().
 */
typedef struct input_s
{
	char keys[NUMBER_OF_KEYS]; /**< Current keymap  */

	int none;
	int left;
	int right;
	int rotate;
	int down;
	int drop;
	int rotate_backw;
	int quit;
	int pause;
	int hold;
	int restart;

} input_s;

/**
 * Container for all info about the game engine.
 */
typedef struct engine_s
{
	input_s  input;
	layout_s layout;

} engine_s;

/** Global engine structure */
engine_s engine;

/* Forward defining some structures to shut up the compiler
 * (they'll be linked on anyway) */
#ifndef PIECE_S_DEFINED
#define PIECE_S_DEFINED
typedef struct piece_s piece_s;
#endif
#ifndef BOARD_S_DEFINED
#define BOARD_S_DEFINED
typedef struct board_s board_s;
#endif
#ifndef GAME_S_DEFINED
#define GAME_S_DEFINED
typedef struct game_s  game_s;
#endif

/**
 * Initializes all ncurses' related stuff (windows, colors...).
 * There's no need to call 'engine_exit'.
 */
bool engine_init();

/** This function blocks the interrupt signal (Ctrl+C) during
 *  the game's initialization.
 *  That's because ncurses leaves the terminal in a broken state.
 */
bool block_signals();

/** Now the player's allowed to interrupt the program it he wishes so. */
bool restore_signals();

/** This stops ncurses on a sweet, gentle way. */
void engine_exit();

/** Function called when receiving an interrupt signal.
 *  It restores the terminal to it's initial state and
 *  frees whatever memory might be allocated from the game.
 */
void engine_safe_exit(int sig);

/**
 * This defines the keymap according to the string #keymap.
 *
 * Each char represents the key to be pressed for the following
 * commands (on that order):
 *
 *     down, right, left, rotate, rotate backwards,
 *     drop, pause, quit, hold
 *
 * For example:
 *    "sdawe pqu"
 */
int engine_keymap(char keymap[]);

/**
 * Get input, waiting at most #delay_ms miliseconds.
 *
 * @return An input enum - it could be ERROR, you know.
 */
int engine_get_input(int delay_ms);

/**
 * Draws a whole piece.
 */
void engine_draw_piece(piece_s* p, WINDOW* w);

/**
 * Draws the board #b.
 */
void engine_draw_board(board_s* b);

void engine_draw_block_theme(WINDOW* w, block_theme_s* t, int x, int y);

/** Prints 'pause' on the board */
void engine_draw_pause();

/** Draws the window where we keep the hold piece */
void engine_draw_hold(game_s* g);

/** Draws the score, high score and warn the player if he has any
 *  combo or back-to-back sequences.
 */
void engine_draw_score(game_s* g);

/** This is a very ugly function that draws blocks on the
 *  info window for statistical purposes.
 *  It uses several position hacks that i need to fix later.
 *  The problem is that pieces always start relative to the
 *  middle of the board, independent of the layout.
 *  So im repositioning them according to it, on the info layout.
 */
void engine_draw_statistics(game_s* g);

/** Calls all drawing routines in order */
void engine_draw(game_s* g);

/** Draws a nice animation when the player loses the game */
void engine_draw_gameover_animation(game_s* g);

/** Prints a message telling the player that
 * he has lost the game.
 */
void engine_draw_gameover();

/** Tries to refresh all windows onlayout */
void engine_refresh_all_windows();

/** Init the help-showing window */
void engine_create_help();

void engine_draw_line_statistics(game_s* g);

/** Pops a window explaining some stuff */
void engine_draw_help();

/** Init the score-showing window */
void engine_create_hscores_window();

/** Deletes the high scores window.
 *  @note This leaves the game layout on a messed-up state. Please
 *        redraw it by calling engine_draw(g) afterwards!
 */
void engine_delete_hscores_window();

/** Deletes the help window.
 *  @note This leaves the game layout on a messed-up state. Please
 *        redraw it by calling engine_draw(g) afterwards!
 */
void engine_delete_help();

void engine_create_input();

void engine_draw_hscores();

void engine_draw_input();

void engine_get_hscore_name(char* name, int size);

void engine_delete_input();

#endif /* ENGINE_H_DEFINED */


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

#ifndef GAME_H_DEFINED
#define GAME_H_DEFINED

/**
 *
 * Here's the order of game termination:
 * - The player loses (board is full, on update()):
 * - we call game_over()
 * - game_over() does one-time settings
 * - game_over() sets the game state to GAME_OVER
 * - while the state is GAME_OVER, we wait for a specific keypress
 *   via game_handle_input()
 * - when we get the keypress we wanted, we set game.is_over to
 *   true, so the main() can see the game has ended.
 * - main() then restarts the game
 */
#include <stdbool.h> /* bool type in C */
#include "globals.h"
#include "piece.h"
#include "board.h"
#include "timer.h"

#define INITIAL_SPEED  1000 /* miliseconds */

/** All the possible game states */
typedef enum { PLAYING, PAUSED, QUITTING, GAME_OVER, HELP,
			   HSCORES, INPUT } game_state;

/** The main game structure. Controls all the actions related to game logic */
struct game_s
{
	/* piece and board stuff */
	piece_s piece_current;
	piece_s piece_next[NEXT_PIECES_NO];
	piece_s piece_ghost;
	piece_s piece_hold;
	board_s board;

	/* timer-related stuff */
	timer_s global_timer; /** Timer since the beginning of the game */
	timer_s piece_timer; /** Timer to drop current piece */
	long gameplay_s; /** How many seconds have passed since game start */
	long gameplay_m; /** How many minutes have passed since game start */
	long gameplay_h; /** How many hours have passed since game start */

	/* player info */
	int  score;              /**< How many points the user has right now */
	int  lines;              /**< How many lines have been cleared yet */
	int  level;              /**< Current level of the game (determines speed) */
	int  speed;              /**< Time in miliseconds between each piece step */
	int  hscore;             /**< Top high score on the list */
	int  combo_count;        /**< How many combos the used currently have */
	int  back_to_back_lines; /**< How many lines have you cleared previously */
	int  back_to_back_count; /**< How many of the same lines have been cleared */
	int  score_delta;        /**< How much score the player won at last lines */

	/* flags */
	bool can_hold;         /**< Has the user switched pieces this round? */
	bool quit;             /**< Will we quit the game */
	bool is_over;          /**< Is the game over */
	bool show_help;        /**< Will the help screen pop up */
	bool show_hscores;
	bool moved_piece_down; /**< Player forced to move down - reset drop timer */
	bool is_combo;         /**< Is the player currently doing a combo */
	bool is_back_to_back;  /**< Is the player currently doing a back-to-back */
	bool show_score_delta; /**< Should we show the score delta onscreen? */

	game_state state; /**< Current game state (paused, running, menu...) */

	/* piece statistics */
	int I_count;
	int T_count;
	int L_count;
	int J_count;
	int S_count;
	int Z_count;
	int O_count;
	int piece_count; /* total count */

	/* line statistics */
	int single_count;
	int double_count;
	int triple_count;
	int tetris_count;
	int lines_count;
};

#ifndef GAME_S_DEFINED
#define GAME_S_DEFINED
typedef struct game_s game_s;
#endif

/**
 * Initializes and returns a new game structure
 * with all it's dependencies
 */
game_s new_game(int x, int y);

/**
 * Refreshes the ghost piece to the current one on #g.
 */
void game_ghost_update(game_s* g);

/**
 * Locks the current piece on the board and gets a new one.
 */
void game_lock_piece(game_s* g);

/**
 * Returns the next piece and refreshes the piece_next array.
 */
piece_s game_get_next_piece(game_s* g);

/**
 * Performs any updates on the game in general.
 */
void game_update(game_s* g);

/**
 * Called once when the player loses.
 *
 * Will set a state GAME_OVER, from where the game should
 * know how to react.
 *
 * Usually it respawns itself or sends to the main menu.
 */
void game_over(game_s* g);

/**
 * Updates piece position on screen.
 *
 * I need the g->moved_piece_down because
 * if the player moved the piece down,
 * I must reset the piece timer (it wont double-drop it).
 *
 * But if the player moved the piece sideways, must avoid infinite
 * floating.
 */
void game_update_piece(game_s* g);

/**
 * Updates level based on the number of lines cleared.
 */
void game_update_level(game_s* g);

/**
 * Updates speed based on the number of lines cleared.
 */
void game_update_speed(game_s* g);

/**
 * Saves current piece for later use.
 * If there's already one on the 'hold slot', switch them.
 *
 * @note Can only do this once per drop.
 */
bool game_hold_piece(game_s* g);


/**
 * Checks all lines, deleting the ones that are full.
 *
 * This function also deals with score. These are the rules:
 *
 * Line_score: `single: 100, double: 300, triple: 500, tetris: 800`
 * Combo_score: `50 * combo_count * current_level`
 * Back_to_back_score: `(line_score * 3) / 2`
 *
 * Score: `(line_score * current_level) + combo_score`
 *
 * Also, when you hard drop a piece, it's 10 points for free.
 *
 * Soft drops are the same
 * (although they should be based on height).
 *
 * @note I know this function's ugly...
 * @todo Maybe create a 'Line' data structure? To make this?
 */
bool game_delete_possible_lines(game_s* g);

/**
 * Updates the time indicator on the right screen.
 */
void game_update_gameplay_time(game_s* g);

/**
 * Perform actions based on #input.
 *
 * @note It's `int` (and not `char`) because of nCurses' stuff.
 */
void game_handle_input(game_s* g, int input);

/**
 * Starts the high score list with default values.
 */
void game_hscore_init(game_s* g);

/**
 * Saves high score if it's big enough.
 */
void game_handle_score(game_s* g);

/**
 * Saves high score - 'nuff said.
 */
void game_hscore_save(game_s* g);

/**
 * Toggles what statistics appear on the info screen.
 */
void game_switch_statistics();

/* experimental features - don't rely on them*/

/**
 * Saves the current game state.
 * Interrupts if something bad happens.
 */
void game_save(game_s* g);

/**
 * Loads a saved game state.
 * Interrupts if something wrong happens.
 */
void game_load(game_s* g);

#endif /* GAME_H_DEFINED */

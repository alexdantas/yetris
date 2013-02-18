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

#ifndef GAME_H_DEFINED
#define GAME_H_DEFINED

#include <stdbool.h> /* bool type in C */
#include "globals.h"
#include "piece.h"
#include "board.h"
#include "timer.h"

#define INITIAL_SPEED  1000 /* miliseconds */

/** All the possible game states */
typedef enum { PLAYING, PAUSED, QUITTING } game_state;

/** The main game structure. Controls all the actions related to game logic */
typedef struct game_s
{
	/* piece and board stuff */
	piece_s piece_current;
	piece_s piece_next[NEXT_PIECES_NO];
	piece_s piece_ghost;
	piece_s piece_hold;
	board_s board;

	/* timer-related stuff */
	struct timert global_timer; /** Timer since the beginning of the game */
	struct timert piece_timer; /** Timer to drop current piece */
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

	/* flags */
	bool can_hold;         /**< Has the user switched pieces this round? */
	bool quit;             /**< Will we quit the game */
	bool is_over;          /**< Is the game over */
	bool show_help;        /**< Will the help screen pop up */
	bool moved_piece_down; /**< Player forced to move down - reset drop timer */
	bool is_combo;         /**< Is the player currently doing a combo */
	bool is_back_to_back;  /**< Is the player currently doing a back-to-back */

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
} game_s;

game_s new_game();
void game_lock_piece(game_s* g);
void game_update(game_s* g);
void game_ghost_update(game_s* g);
bool game_hold_piece(game_s* g);
bool game_delete_possible_lines(game_s* g);
void game_handle_input(game_s* g, int input);
void game_hscore_init(game_s* g);
void game_handle_score(game_s* g);
void game_over(game_s* g);
void game_update_gameplay_time(game_s* g);
void game_update_level(game_s* g);
void game_update_piece(game_s* g);
void game_hscore_save(game_s* g);
piece_s game_get_next_piece(game_s* g);

#endif /* GAME_H_DEFINED */

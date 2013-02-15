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

#include <stdbool.h>
#include "piece.h"
#include "board.h"
#include "timer.h"

#define INITIAL_SPEED  1000 /* miliseconds */

typedef struct game_s
{
	piece_s piece_current;
	piece_s piece_next[NEXT_PIECES_NO]; /* 1 current and 4 nexts */
	piece_s piece_ghost;   /* Indicating where the piece will land */
	piece_s piece_hold;
	board_s board;

	struct timert global_timer; /** Timer since the beginning of the game */
	struct timert piece_timer; /** Timer to drop current piece */
	long gameplay_s; /** How many seconds have passed since game start */
	long gameplay_m; /** How many minutes have passed since game start */
	long gameplay_h; /** How many hours have passed since game start */

	bool can_hold; /**< Tells if user has switched pieces this round */
	int  score;
	int  lines; /**< How many lines have been cleared yet */
	int  level;
	int  speed; /**< Time in miliseconds between each piece step */
	int  hscore;
	int  combo_count;
	int  back_to_back_lines; /* How many lines have you cleared last round */
	int  back_to_back_count; /* How many of the same lines have been cleared */

	bool quit;
	bool is_over; /**< Flag if game is over  */
	bool show_help;
	bool moved_piece_down; /**< Player forced to move down - wont drop it */
	bool is_combo;
	bool is_back_to_back;

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


#ifndef GAME_H_DEFINED
#define GAME_H_DEFINED

#include <stdbool.h>
#include "piece.h"
#include "board.h"
#include "timer.h"

#define NEXT_PIECES_NO 5
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

	bool quit;
	bool is_over; /**< Flag if game is over  */
	bool show_help;
	bool moved_piece_down; /**< Player forced to move down - wont drop it */

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

#endif /* GAME_H_DEFINED */

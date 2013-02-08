
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

	struct timert timer;

	bool can_hold; /**< Tells if user has switched pieces this round */
	int  score;
	int  lines; /**< How many lines have been cleared yet */
	int  level;
	int  speed; /**< Time in miliseconds between each piece step */

	bool quit;
	bool over; /**< Flag if game is over  */
} game_s;

game_s new_game();
void game_save_piece(game_s* g);
void game_update(game_s* g);
void game_ghost_update(game_s* g);
bool game_hold_piece(game_s* g);
void game_delete_possible_lines(game_s* g);
void game_update_speed(game_s* g);
void game_handle_input(game_s* g, int input);

#endif /* GAME_H_DEFINED */

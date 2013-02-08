
#ifndef GAME_H_DEFINED
#define GAME_H_DEFINED

#include <stdbool.h>
#include "piece.h"
#include "board.h"

typedef struct game_s
{
	piece_s* piece_current;
	piece_s  piece_next[5]; /* 1 current and 4 nexts */
	piece_s  piece_ghost;   /* Indicating where the piece will land */
	piece_s  piece_hold;
	board_s  board;

	bool     can_hold; /* Tells if user has switched pieces this round */
	int      score;
	int      lines; /* how many lines have been cleared yet */
	int      level;

} game_s;

game_s new_game();
void game_drop_piece(game_s* g);
bool game_is_over(game_s* g);
void game_update(game_s* g);
void game_ghost_update(game_s* g);
bool game_hold_piece(game_s* g);
void game_delete_possible_lines(game_s* g);

#endif /* GAME_H_DEFINED */

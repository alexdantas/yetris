
#include <stdbool.h>
#include "game.h"
#include "piece.h"
#include "board.h"
#include "engine.h"

/** Initializes and returns a new game structure with all it's dependencies */
game_s new_game()
{
	game_s g;

	g.board = new_board();

	int i;
	for (i = 0; i < 5; i++)
		g.piece_next[i] = new_piece(piece_get_random());

	g.piece_current = &(g.piece_next[0]);

	game_ghost_update(&g);

	return g;
}

/** Refreshes the ghost piece to the current one on #g */
void game_ghost_update(game_s* g)
{
	g->piece_ghost = *(g->piece_current);

	int i;
	for (i = 0; i < 4; i++)
	{
		g->piece_ghost.block[i].type  = EMPTY;
		g->piece_ghost.block[i].color = WHITE_BLACK;
	}
	g->piece_ghost.color = WHITE_BLACK;

	piece_hard_drop(&(g->piece_ghost), &(g->board));
}

/** Places the current piece on the board and gets a new one. */
void game_drop_piece(game_s* g)
{
	board_save_piece(&(g->board), g->piece_current);

	int i;
	for (i = 0; i < 4; i++)
		g->piece_next[i] = g->piece_next[i + 1];

	g->piece_next[i] = new_piece(piece_get_random());
	g->piece_current = &(g->piece_next[0]);

	game_ghost_update(g);
}

/** Calls all drawing routines in order */
void game_draw(game_s* g)
{
	werase(engine.screen.board);

	engine_draw_board(&(g->board));
	engine_draw_piece(&(g->piece_ghost));
	engine_draw_piece(g->piece_current);

	wrefresh(engine.screen.board);
}

/** Tests if the game is over */
bool game_is_over(game_s* g)
{
	return board_is_full(&(g->board));
}

/** Perform any updates on the data structures inside #g */
void game_update(game_s* g)
{
	board_delete_possible_lines(&(g->board));
	game_ghost_update(g);
	if (!piece_can_move(g->piece_current, &(g->board), DIR_DOWN))
		game_drop_piece(g);
}


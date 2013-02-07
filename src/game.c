
#include <stdbool.h>
#include "game.h"
#include "piece.h"
#include "board.h"
#include "engine.h"

game_s new_game()
{
	game_s g;

	g.board = new_board();

	int i;
	for (i = 0; i < 5; i++)
		g.piece_next[i] = new_piece(piece_get_random());

	g.piece_current = &(g.piece_next[0]);

	return g;
}

void game_drop_piece(game_s* g)
{
	board_save_piece(&(g->board), g->piece_current);

	int i;
	for (i = 0; i < 4; i++)
		g->piece_next[i] = g->piece_next[i + 1];

	g->piece_next[i] = new_piece(piece_get_random());
	g->piece_current = &(g->piece_next[0]);
}

void game_draw(game_s* g)
{
	werase(engine.screen.board);

	engine_draw_board(&(g->board));
	engine_draw_piece(g->piece_current);

	wrefresh(engine.screen.board);
}

bool game_is_over(game_s* g)
{
	int i;
	for (i = 0; i < BOARD_WIDTH; i++)
		if (g->board.block[i][0].type != EMPTY)
			return true;

	return false;
}

void game_update(game_s* g)
{

	board_delete_possible_lines(&(g->board));
}


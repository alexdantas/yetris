

typedef struct game_s
{
	piece_s* piece_current;
	piece_s  piece_next[5]; /* 1 current and 4 nexts */
	board_s  board;

} game_s;

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

	engine_draw_piece(g->piece_current);
	engine_draw_board(&(g->board));

	wrefresh(engine.screen.board);
}


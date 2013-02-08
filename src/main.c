
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "arguments.h"
#include "engine.h"
#include "block.h"
#include "piece.h"
#include "board.h"
#include "game.h"
#include "globals.h"

int main(int argc, char* argv[])
{
	global = new_globals();

	/* if i call this after starting the engine, printf doesnt work
	 * for help and usage */
	args_handle(argc, argv);

	engine_init();
	atexit(engine_exit);

	game_s game = new_game();

	bool quit = false;
	while (!quit)
	{
		int c = engine_get_input(300);

		/* Dang it, switch doesn't work with variables! */
		if (c == engine.input.quit)
			quit = 1;
		else if (c == engine.input.left)
		{
			if (piece_can_move(game.piece_current, &(game.board), DIR_LEFT))
				piece_move(game.piece_current, DIR_LEFT);
		}
		else if (c == engine.input.right)
		{
			if (piece_can_move(game.piece_current, &(game.board), DIR_RIGHT))
				piece_move(game.piece_current, DIR_RIGHT);
		}
		else if (c == engine.input.down)
		{
			if (piece_can_move(game.piece_current, &(game.board), DIR_DOWN))
				piece_move(game.piece_current, DIR_DOWN);
			else
				game_drop_piece(&game);
		}
		else if (c == engine.input.rotate)
		{
			if (piece_can_rotate(game.piece_current, &(game.board), 1))
				piece_rotate(game.piece_current, 1);
		}
		else if (c == engine.input.rotate_backw)
		{
			if (piece_can_rotate(game.piece_current, &(game.board), -1))
				piece_rotate(game.piece_current, -1);
		}
		else if (c == engine.input.drop)
		{
			piece_hard_drop(game.piece_current, &(game.board));
			game_drop_piece(&game);
		}
		else if (c == engine.input.pause)
		{
			game_hold_piece(&game);
		}

		game_update(&game);
		engine_draw(&game);

		if (game_is_over(&game))
			quit = true;
	}
	return 0;
}


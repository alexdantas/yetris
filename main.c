
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "arguments.h"
#include "engine.c"
#include "block.c"
#include "piece.c"
#include "board.c"
#include "game.c"

int main(int argc, char* argv[])
{
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
			piece_move(game.piece_current, 1);

		else if (c == engine.input.right)
			piece_move(game.piece_current, 0);

		else if (c == engine.input.down)
			piece_soft_drop(game.piece_current);

		else if (c == engine.input.rotate)
			piece_rotate(game.piece_current, 1);

		else if (c == engine.input.rotate_backw)
			piece_rotate(game.piece_current, -1);

		else if (c == engine.input.drop)
			game_drop_piece(&game);

		else if (c == engine.input.pause)
			board_delete_line(&(game.board), 5);

		game_update(&game);
		game_draw(&game);

		if (game_is_over(&game))
			quit = true;
	}
	return 0;
}


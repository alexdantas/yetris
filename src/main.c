
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

	while (!game.quit)
	{
		int c = engine_get_input(game.speed);

		game_handle_input(&game, c);
		game_update(&game);

		if (game.over)
		{
			engine_draw_gameover(&game);
			engine_wait_for_keypress();
			game = new_game();
		}

		engine_draw(&game);
	}
	return 0;
}


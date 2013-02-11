
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

	args_handle(argc, argv);

	engine_init();
	atexit(engine_exit); /* no need to call it when quitting */

	game_s game = new_game();
	game_hscore_init(&game);

	while (!game.quit)
	{
		int c = engine_get_input(game.speed);

		game_handle_input(&game, c);
		game_update(&game);

		/* if (game.show_help) */
		/* { */
		/* 	engine_draw_help(); */
		/* 	engine_wait_for_keypress(); */
		/* 	game.show_help = false; */
		/* } */
		if (game.over)
		{
			engine_draw_gameover(&game);
			engine_wait_for_keypress();
			game_handle_score(&game);
			game = new_game();
		}
		engine_draw(&game);
	}

	return EXIT_SUCCESS;
}



#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* for now i must keep this - windows doesnt have getopt */
#if !OS_IS_WINDOWS
	#include "arguments.h"
#endif

#include "engine.h"
#include "block.h"
#include "piece.h"
#include "board.h"
#include "game.h"
#include "globals.h"
#include "config.h"
#include "hscore.h"

/** Main entry point for the game. I kept it as simple as I could. */
int main(int argc, char* argv[])
{
	global = new_globals();

	config_handle();

/* Windows doesnt support getopt_long(). Ill need to work around this somehow */
#if !OS_IS_WINDOWS
	args_handle(argc, argv);
#endif

	engine_init();
	atexit(engine_exit); /* no need to call it when quitting */
	hscore_init();
	hscore_load();

	game_s game = new_game();
	engine_draw(&game);

	while (!game.quit)
	{
		int c = engine_get_input(game.speed);

		game_handle_input(&game, c);
		game_update(&game);

		if (game.is_over)
			game = new_game();

		engine_draw(&game);
	}

	return EXIT_SUCCESS;
}


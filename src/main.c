/* yetris - Tetris(tm) on the console.
 * Copyright (C) 2013 Alexandre Dantas (kure)
 *
 * This file is part of yetris.
 *
 * yetris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://www.github.com/alexdantas/yetris/
 * mailto:   alex.dantas92@gmail.com
 */

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

/* How many microseconds are there in a frame (supposing I want 60 FPS */
#define UFPS (1000000/60)


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

	game_s game = new_game();
	engine_draw(&game);


	timer_s fps_timer;
	timer_start(&fps_timer);
	while (!game.quit)
	{
		int c = engine_get_input(game.speed);

		game_handle_input(&game, c);
		game_update(&game);

		if (game.is_over)
			game = new_game();

		engine_draw(&game);


		timer_stop(&fps_timer);
		if (timer_delta_useconds(&fps_timer) < UFPS)
		{
			long delay = UFPS - timer_delta_useconds(&fps_timer);
			usleep(delay);
		}
		timer_start(&fps_timer);
	}

	return EXIT_SUCCESS;
}


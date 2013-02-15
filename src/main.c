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

#include "arguments.h"
#include "engine.h"
#include "block.h"
#include "piece.h"
#include "board.h"
#include "game.h"
#include "globals.h"

/** Main entry point for the game.
 *  I tried to keep it as simple as I could. */
int main(int argc, char* argv[])
{
	global = new_globals();

	args_handle(argc, argv);

	engine_init();
	atexit(engine_exit); /* no need to call it when quitting */

	game_s game = new_game();

	while (!game.quit)
	{
		int c = engine_get_input(game.speed);

		game_handle_input(&game, c);
		game_update(&game);

		/* THIS WILL BE UNCOMMENTED SOON */
		/* if (game.show_help) */
		/* { */
		/* 	engine_draw_help(); */
		/* 	engine_wait_for_keypress(); */
		/* 	game.show_help = false; */
		/* } */
		if (game.is_over)
		{
			game_over(&game);
			engine_draw_gameover(&game);
			engine_wait_for_keypress();
			game = new_game();
		}
		engine_draw(&game);
	}

	return EXIT_SUCCESS;
}


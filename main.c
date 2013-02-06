
#include <stdlib.h>
#include <stdbool.h>
#include "arguments.h"
#include "engine.c"
#include "game.c"
#include "block.c"
#include "piece.c"
#include "board.c"

int main(int argc, char* argv[])
{
	/* if i call this after starting the engine, printf doesnt work
	 * for help and usage */
	args_handle(argc, argv);

	engine_init();
	atexit(engine_exit);

	piece_s piece = new_piece(PIECE_T);

	bool quit = false;
	while (!quit)
	{
		werase(engine.screen.board);
		int c = engine_get_input(300);

		/* Dang it, switch doesn't work with variables! */
		if (c == engine.input.quit)
			quit = 1;
		else if (c == engine.input.left)
			piece_move(&piece, 1);
		else if (c == engine.input.right)
			piece_move(&piece, 0);
		else if (c == engine.input.down)
			piece_soft_drop(&piece);
		else if (c == engine.input.rotate)
			piece_rotate(&piece, 1);
		else if (c == engine.input.rotate_backw)
			piece_rotate(&piece, -1);

		engine_draw_piece(&piece);
		wrefresh(engine.screen.board);
	}
	return 0;
}


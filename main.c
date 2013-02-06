
#include <stdlib.h>
#include "arguments.h"
#include "engine.c"

int main(int argc, char* argv[])
{
	int quit = 0;

	/* if i call this after starting the engine, printf doesnt work
	 * for help and usage */
	args_handle(argc, argv);

	engine_init();
	atexit(engine_exit);

	while (!quit)
	{
		int c = 0;
		c = engine_get_input(300);
		/* Damn, switch doesn't work with variables! */
		if (c == engine.input.quit)
			quit = 1;
	}
	return 0;
}


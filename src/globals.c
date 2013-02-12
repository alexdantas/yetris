
#include "globals.h"

globals_s new_globals()
{
	globals_s g;

	g.screen_center_vertically   = false;
	g.screen_center_horizontally = false;
	g.screen_use_colors = true;

	return g;
}


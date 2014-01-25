
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "engine.h"

void block_theme_set(block_theme_s* t, color_pair_t color, char one, char two)
{
	t->color = color;
	t->appearance[0] = one;
	t->appearance[1] = two;
}

globals_s new_globals()
{
	globals_s g;

	g.screen_center_vertically	    = false;
	g.screen_center_horizontally    = false;
	g.screen_use_colors			    = true;
    g.screen_use_default_background = true;
	g.screen_fancy_borders		    = true;
	g.screen_show_outer_border	    = true;

	g.game_has_ghost               = true;
	g.game_can_hold                = true;
	g.game_next_no                 = NEXT_PIECES_NO;
	g.game_has_statistics          = true;
	g.game_has_line_statistics     = false;
	g.game_random_algorithm        = 1;
	g.game_line_clear_timeout      = 200000;
	g.game_has_game_over_animation = true;

	g.theme_piece_has_colors = true;
	g.theme_ghost_has_colors = false;
	g.theme_show_pivot_block = false;
	g.theme_lock_piece_color = false;

	/* the hard-coded default appearances */
	block_theme_set(&(g.theme_clear_line),
	                color_pair(COLOR_WHITE, COLOR_DEFAULT, false),
	                ':', ':');

	block_theme_set(&(g.theme_piece_colorless),
	                color_pair(COLOR_BLACK, COLOR_WHITE, false),
	                ' ', ' ');

	block_theme_set(&(g.theme_piece),
	                color_pair(COLOR_BLACK, COLOR_WHITE, false),
	                ' ', ' ');

	block_theme_set(&(g.theme_ghost),
	                color_pair(COLOR_BLACK, COLOR_WHITE, false),
	                '[', ']');

	block_theme_set(&(g.theme_locked),
	                color_pair(COLOR_WHITE, COLOR_WHITE, false),
	                ' ', ' ');

	block_theme_set(&(g.theme_piece_S),
	                color_pair(COLOR_WHITE, COLOR_GREEN, false),
	                ' ', ' ');

	block_theme_set(&(g.theme_piece_Z),
	                color_pair(COLOR_WHITE, COLOR_RED, false),
	                ' ', ' ');

	block_theme_set(&(g.theme_piece_O),
	                color_pair(COLOR_WHITE, COLOR_YELLOW, false),
	                ' ', ' ');

	block_theme_set(&(g.theme_piece_I),
	                color_pair(COLOR_WHITE, COLOR_CYAN, true),
	                ' ', ' ');

	block_theme_set(&(g.theme_piece_L),
	                color_pair(COLOR_WHITE, COLOR_YELLOW, true),
	                ' ', ' ');

	block_theme_set(&(g.theme_piece_J),
	                color_pair(COLOR_WHITE, COLOR_BLUE, false),
	                ' ', ' ');

	block_theme_set(&(g.theme_piece_T),
	                color_pair(COLOR_WHITE, COLOR_MAGENTA, false),
	                ' ', ' ');

	g.fps = 0;

	memset(g.config_filename, '\0', 256);
	memset(g.hscore_filename, '\0', 256);

	return g;
}


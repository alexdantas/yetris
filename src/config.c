/* @file config.h
 * This file implements stuff related to the configuration file.
 * It can read customizations on it and set the global variables.
 *
 * If the user specifies options at commandline, they have higher
 * precedence over the config file.
 *
 * Remember, DEFAULT_CONFIG_FILE is defined on the Makefile.
 * By default, it's '/home/<user>/.yetrisrc.ini'
 *
 */

#include <string.h>
#include "config.h"
#include "globals.h"
#include "engine.h"

int get_color_from_string(char* string)
{
	if (!string) return -1;

	if (strcasecmp(string, "black")   == 0) return COLOR_BLACK;
	if (strcasecmp(string, "red")     == 0) return COLOR_RED;
	if (strcasecmp(string, "green")   == 0) return COLOR_GREEN;
	if (strcasecmp(string, "yellow")  == 0) return COLOR_YELLOW;
	if (strcasecmp(string, "blue")    == 0) return COLOR_BLUE;
	if (strcasecmp(string, "magenta") == 0) return COLOR_MAGENTA;
	if (strcasecmp(string, "cyan")    == 0) return COLOR_CYAN;
	if (strcasecmp(string, "white")   == 0) return COLOR_WHITE;

	return -1;
}

/** Deals with the config file, storing each option in memory.
 *   @see globals.h
 */
void config_handle()
{
	// try to deal with custom config name specified by the player
	if (!config_file_exists(DEFAULT_CONFIG_FILE))
	{
		config_create_default(DEFAULT_CONFIG_FILE);
		return;
	}

	config_parse(DEFAULT_CONFIG_FILE);
}

/** Tests if #filename exists. */
bool config_file_exists(char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
		return false;

	fclose(file);
	return true;
}

/** Reads the config file and stores It's contents in memory
 *  This is the main interface with the iniparser library.
 *  Watch out, this function's big and scary, but you can do it
 *  if you take your time and go step-by-step.
 *
 *  Warning, macros ahead!
 */
void config_parse(char* filename)
{
	globals_s* g = &global;

	/* Actually starting the parser */
	dictionary* ini = iniparser_load(filename);
	if (!ini) return; /* couldn't parse */


/* This macro sets the boolean variable ONLY if it's valid (0 or 1).
 * It'll look on the config file and search for the config string.
 * I know it's kinda ugly, but it avoid code repetition.
 *
 * Note that it depends on the 'bool b' variable!
 */
	bool b;
#define set_bool_if_valid(var, string)              \
	{                                               \
		b = -1;                                     \
		b = iniparser_getboolean(ini, string, -1);  \
		if ((b != -1) && ((b == 0) || (b == 1)))    \
			var = b;                                \
	}
	set_bool_if_valid(g->screen_use_colors,          "interface:colors");
	set_bool_if_valid(g->screen_center_vertically,   "interface:center_vertical");
	set_bool_if_valid(g->screen_center_horizontally, "interface:center_horizontal");
	set_bool_if_valid(g->screen_fancy_borders,       "interface:fancy_borders");
	set_bool_if_valid(g->screen_show_outer_border,   "interface:outer_border");
	set_bool_if_valid(g->game_has_statistics,        "interface:statistics");

	set_bool_if_valid(g->game_can_hold,  "gameplay:hold");
	set_bool_if_valid(g->game_has_ghost, "gameplay:ghost");

	set_bool_if_valid(g->theme_piece_has_colors, "theming:piece_has_color");
	set_bool_if_valid(g->theme_ghost_has_colors, "theming:ghost_has_color");
	set_bool_if_valid(g->theme_show_pivot_block, "theming:show_pivot");
	set_bool_if_valid(g->theme_lock_piece_color, "theming:locked_piece_color");


/* This other macro sets the integer variable ONLY if
 * specified condition checks.
 * It'll look on the config file and search for the config string.
 * This one is even uglier than the boolean one.
 *
 * Note that it depends on the 'int i' variable!
 */
	int i;
#define set_int_if(condition, var, string)     \
	{                                          \
		i = 0;                                 \
		i = iniparser_getint(ini, string, -1); \
		if (condition)                         \
			var = i;                           \
	}
	set_int_if((i >= 0) && (i <= NEXT_PIECES_NO), g->game_next_no, "gameplay:next");
	set_int_if((i >= 1) && (i <= 2), g->game_random_algorithm, "gameplay:random");


/* Finally, this one's way more specific than the others.
 * It only works to get the piece's theme, which consists of a
 * 2-char string.
 * So, if I try to get a string from the config and it's null,
 * I won't add to the theme. Just like that.
 *
 * I dont even have to say that 'char* s' is essential.
 */
	char* s;
#define set_theme_if_valid(dest, config_string)            \
	{                                                      \
		s = NULL;                                          \
		s = iniparser_getstring(ini, config_string, NULL); \
		if ((s) && (s[0] != '\0'))                         \
		{                                                  \
			dest[0] = s[0];                                \
			dest[1] = s[1];                                \
		}                                                  \
	}
	set_theme_if_valid(g->theme_piece, "theming:piece");
	set_theme_if_valid(g->theme_ghost, "theming:ghost");
	set_theme_if_valid(g->theme_clear_line, "theming:clear_line");

	/* I'm doing this so if the player doesn't specify
	 * a piece's theme, it'll fallback to the one
	 * he specified to be the default for all pieces.
	 *
	 * If he doesn't specify one at all, it'll be the
	 * hardcoded default, anyway.
	 */
	int j;
	for (j = 0; j < 2; j++)
	{
		g->theme_piece_S[j] = g->theme_piece[j];
		g->theme_piece_Z[j] = g->theme_piece[j];
		g->theme_piece_O[j] = g->theme_piece[j];
		g->theme_piece_I[j] = g->theme_piece[j];
		g->theme_piece_L[j] = g->theme_piece[j];
		g->theme_piece_J[j] = g->theme_piece[j];
		g->theme_piece_T[j] = g->theme_piece[j];
	}

	/* Finally I'll get the piece-specific theme from the config */
	set_theme_if_valid(g->theme_piece_S, "theming:piece_S");
	set_theme_if_valid(g->theme_piece_Z, "theming:piece_Z");
	set_theme_if_valid(g->theme_piece_O, "theming:piece_O");
	set_theme_if_valid(g->theme_piece_I, "theming:piece_I");
	set_theme_if_valid(g->theme_piece_L, "theming:piece_L");
	set_theme_if_valid(g->theme_piece_J, "theming:piece_J");
	set_theme_if_valid(g->theme_piece_T, "theming:piece_T");


	/* This gets all piece-specific colors from the config file.
	 * I get the string ("black") and convert it to my color value
	 */
	int fg , bg, new_fg, new_bg;
#define get_colors_if_valid(var, string_fg, string_bg, default_fg, default_bg) \
{                                                                              \
	fg = default_fg;                                                           \
	bg = default_bg;                                                           \
	new_fg = get_color_from_string(iniparser_getstring(ini, string_fg, NULL)); \
	if (new_fg != -1) fg = new_fg;                                             \
	new_bg = get_color_from_string(iniparser_getstring(ini, string_bg, NULL)); \
	if (new_bg != -1) bg = new_bg;                                             \
	                                                                           \
		var = engine_get_color(fg, bg, false);                                 \
}
	get_colors_if_valid(g->theme_ghost_color,   "theming:ghost_fg",   "theming:ghost_bg",   COLOR_WHITE, COLOR_BLACK);
	get_colors_if_valid(g->theme_piece_S_color, "theming:piece_S_fg", "theming:piece_S_bg", COLOR_WHITE, COLOR_RED);
	get_colors_if_valid(g->theme_piece_Z_color, "theming:piece_Z_fg", "theming:piece_Z_bg", COLOR_WHITE, COLOR_GREEN);
	get_colors_if_valid(g->theme_piece_O_color, "theming:piece_O_fg", "theming:piece_O_bg", COLOR_WHITE, COLOR_YELLOW);
	get_colors_if_valid(g->theme_piece_I_color, "theming:piece_I_fg", "theming:piece_I_bg", COLOR_WHITE, COLOR_CYAN);
	get_colors_if_valid(g->theme_piece_L_color, "theming:piece_L_fg", "theming:piece_L_bg", COLOR_WHITE, COLOR_YELLOW);
	get_colors_if_valid(g->theme_piece_J_color, "theming:piece_J_fg", "theming:piece_J_bg", COLOR_WHITE, COLOR_BLUE);
	get_colors_if_valid(g->theme_piece_T_color, "theming:piece_T_fg", "theming:piece_T_bg", COLOR_WHITE, COLOR_MAGENTA);

	/* finishing parsing */
	iniparser_freedict(ini);
}

/** Creates a default config file on #filename */
void config_create_default(char* filename)
{
	FILE* file = fopen(filename, "w");

	if (!file)
		return;

	const char text[] =
		"# This is the default yetris configuration file.\n"
		"# Remember that commandline options take precedence over this file.\n"
		"#\n"
		"# Everything after '#' is ignored by the game.\n"
		"# Options are categorized like [this] and have 'key = value' pairs.\n"
		"# If some value is mistyped or left blank, the default value will be used.\n"
		"#\n"
		"# Boolean values accepted are case-insensitive (true, TRUE, tRuE...)\n"
		"#\n"
		"# Each piece is referred by names resembling it's appearance:\n"
		"#\n"
		"#                       []\n"
		"#                       []  []      []\n"
		"#   [][]  [][]    [][]  []  []      []    []\n"
		"# [][]      [][]  [][]  []  [][]  [][]  [][][]\n"
		"#   S        Z     O     I    L    J      T\n"
		"\n"
		"[gameplay]\n"
		"\n"
		"# Enable/disable ghost piece (indication of where the piece will land)\n"
		"# default: true\n"
		"ghost = true\n"
		"\n"
		"# Enable/disable holding a piece (saving current piece for later use)\n"
		"# default: true\n"
		"hold  = true\n"
		"\n"
		"# How many next pieces are shown\n"
		"# default: 6\n"
		"next  = 6\n"
		"\n"
		"# The piece-generation algorithm used by the game. Invalid values\n"
		"# will fallback to the last valid one.\n"
		"# Valid values are:\n"
		"#  1: bag generator\n"
		"#  2: dummy random (srand)\n"
		"# default: 1\n"
		"random = 1\n"
		"\n"
		"[interface]\n"
		"\n"
		"# Enable/disable colors on the game.\n"
		"# default: true\n"
		"colors = true\n"
		"\n"
		"# Center vertically/horizontally the game screen.\n"
		"# default: false/false\n"
		"center_vertical   = false\n"
		"center_horizontal = false\n"
		"\n"
		"# If the game boxes have fancy borders\n"
		"# default: true\n"
		"fancy_borders = true\n"
		"\n"
		"# Show/hide outer border on the game screen\n"
		"# default: true\n"
		"outer_border = true\n"
		"\n"
		"# Show/hide piece statistics\n"
		"# default: true\n"
		"statistics = true\n"
		"\n"
		"[theming]\n"
		"\n"
		"# The appearance of a normal piece (two-char string)\n"
		"# default: \"  \"\n"
		"piece = \"  \"\n"
		"\n"
		"# The appearance of the ghost piece (two-char string)\n"
		"# default: \"[]\"\n"
		"ghost = \"[]\"\n"
		"\n"
		"# If normal pieces have colors (each individual color is defined by default)\n"
		"# default: true\n"
		"piece_has_color = true\n"
		"\n"
		"# If the ghost color has color\n"
		"# (it's color would be the same as the current piece's).\n"
		"# If 'piece_has_color' is false, the ghost gets the color the current piece\n"
		"# would have.\n"
		"# default: false\n"
		"ghost_has_color = false\n"
		"\n"
		"# The appearance of the line we clear, just before it disappears\n"
		"# default: \"::\"\n"
		"clear_line = \"::\"\n"
		"\n"
		"# Show/hide the pivot block (the center of the piece's rotation axis)\n"
		"# default: false\n"
		"show_pivot = false\n"
		"\n"
		"# When the piece locks on the board, will it have a custom color?\n"
		"# default: false\n"
		"locked_piece_color = false\n"
		"\n"
		"# Colors of the pieces.\n"
		"# They're divided into foreground (fg) and background (bg).\n"
		"#\n"
		"# Options accepted (case-insensitive) are:\n"
		"#\n"
		"#     black, red, green, yellow, blue, magenta, cyan, white\n"
		"#\n"
		"# For default, leave blank\n"
		"\n"
		"# default: white/black\n"
		"#ghost_fg   = white\n"
		"#ghost_bg   = black\n"
		"# default: white/red\n"
		"#piece_S_fg = red\n"
		"#piece_S_bg = black\n"
		"# default: white/green\n"
		"#piece_Z_fg = green\n"
		"#piece_Z_bg = black\n"
		"# default: white/yellow\n"
		"#piece_O_fg = yellow\n"
		"#piece_O_bg = black\n"
		"# default: white/cyan\n"
		"#piece_I_fg = cyan\n"
		"#piece_I_bg = black\n"
		"# default: white/yellow\n"
		"#piece_L_fg = yellow\n"
		"#piece_L_bg = black\n"
		"# default: white/blue\n"
		"#piece_J_fg = blue\n"
		"#piece_J_bg = black\n"
		"# default: white/magenta\n"
		"#piece_T_fg = magenta\n"
		"#piece_T_bg = black\n"
		"\n"
		"# The specific appearance of each piece.\n"
		"# Note that they must come between quotes ("").\n"
		"#\n"
		"# If invalid or not specified, will use the one on 'piece'.\n"
		"# If that one's not specified, will use default.\n"
		"#piece_S = \"SS\"\n"
		"#piece_Z = \"ZZ\"\n"
		"#piece_O = \"OO\"\n"
		"#piece_I = \"II\"\n"
		"#piece_L = \"LL\"\n"
		"#piece_J = \"JJ\"\n"
		"#piece_T = \"TT\"\n"
		"\n";

	/* well, that was easy, wasn't it? */
	fprintf(file, text);
}


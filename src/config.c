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
 *  This is the main interface with the iniparser library
 */
void config_parse(char* filename)
{
	globals_s*  g = &global; /* simple alias */

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
		b = iniparser_getboolean(ini, string, -1);	\
		if ((b != -1) && ((b == 0) || (b == 1)))	\
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
		i = iniparser_getint(ini, string, -1); \
		if (condition)                         \
			var = i;                           \
	}
	set_int_if((i >= 0) && (i <= NEXT_PIECES_NO), g->game_next_no, "gameplay:next");
	set_int_if((i >= 1) && (i <= 2), g->game_random_algorithm, "gameplay:random");

/* Finally, this one's way more specific than the others.
 * It only works to get the piece's theme, which consists of a
 * 2-char string and a trailing null byte.
 * So, if I try to get a string from the config and it's null,
 * I won't add to the theme. Just like that.
 *
 * I dont even have to say that 'char s[3]' is essential.
 */
	char s[3] = {};
#define set_3_char_string_if_not_null(dest, config_string)            \
	{                                                                 \
		strncpy(s, iniparser_getstring(ini, config_string, NULL), 3); \
		if (s[0] != '\0')                                             \
			strncpy(dest, s, 2);                                      \
	}
	set_3_char_string_if_not_null(g->theme_piece, "theming:piece");
	set_3_char_string_if_not_null(g->theme_ghost, "theming:ghost");
	set_3_char_string_if_not_null(g->theme_clear_line, "theming:clear_line");

	//attempt to parse colors
/* 	/\* getting colors *\/ */
/* 	int color; */
/* #define apply_if_valid(string, ok) { (color = iniparser_getint(ini, string, -1)        \ */
/* 		                             if ((color >= 0) && (color < 8)) { ok = color } } */

/* 	apply_if_valid("theming:color_titles_fg", global. */
/* 	color = iniparser_getint(ini, "theming:color_titles_fg", -1); */
/* 	if (is_valid(color)) global */

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
		"\n";

	fprintf(file, text);
}


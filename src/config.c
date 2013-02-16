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

#include "config.h"
#include "globals.h"


/** Deals with the config file, storing each option in memory.
 *  @see globals.h
 */
void config_handle()
{
//	if (global.config_filename != NULL)
//		config_parse(global.config_filename)
//	else
	if (!config_file_exists(DEFAULT_CONFIG_FILE))
//		config_create_default(DEFAULT_CONFIG_FILE);
		return;

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
	dictionary* ini;

	ini = iniparser_load(filename);
	if (!ini)
		return; /* couldnt parse */

	global.screen_use_colors = iniparser_getboolean(ini, "interface:colors", -1);

	global.game_can_hold  = iniparser_getboolean(ini, "gameplay:hold",  -1);
	global.game_has_ghost = iniparser_getboolean(ini, "gameplay:ghost", -1);
	global.game_next_no   = iniparser_getint(ini,     "gameplay:next",  -1);

	iniparser_freedict(ini);
}

/** Creates a default config file on #filename */
void config_create_default(char* filename)
{
	FILE* file = fopen(filename, "w");

	if (!file)
		return;

	const char text[] =
		"#\n"
		"# This is the default yetris configuration file\n"
		"#\n"
		"\n"
		"[gameplay]\n"
		"\n"
		"ghost = true\n"
		"hold = true\n"
		"next = 7\n"
		"\n"
		"[interface]\n"
		"\n"
		"colors = true\n"
		"\n";

	fprintf(file, text);
}


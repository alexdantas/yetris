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
 * homepage: http://yetris.alexdantas.net
 * mailto:   eu@alexdantas.net
 */

#ifndef CONFIG_H_DEFINED
#define CONFIG_H_DEFINED

/**
 * @file config.h
 * Interface to the configuration file.
 *
 * @note We expect two definitions from the Makefile,
 *       `PACKAGE` which is the program name and
 *       `CONFIG_FILE` which is the name of the config file.
 *
 * So, the default configuration file is at:
 * `$(HOME)/.$(PACKAGE)/$(CONFIG_FILE)`.
 *
 * It uses `iniparser` to read an .ini file and
 * stores the results on `globals.h`.
 * @see globals.h
 *
 * If the user specifies options at commandline,
 * they have higher precedence over the config file.
 */

#include <stdbool.h>
#include "iniparser/iniparser.h"

/**
 * Deals with the config file, storing each option in memory.
 * @see globals.h
 */
void config_handle();

/**
 * Tests if #filename exists.
 */
bool config_file_exists(char* filename);

/**
 *	Big function that reads the config file (#filename) and
 *	stores it's contents in memory.
 *
 *	This is the main interface with the iniparser library.
 *
 *	Watch out, this function's big and scary, but you can do it
 *	if you take your time and go step-by-step.
 *
 *	@warning Macros ahead!
 */
void config_parse(char* filename);

/**
 * Creates a default configuration file on #filename.
 */
void config_create_default(char* filename);

#endif /* CONFIG_H_DEFINED */


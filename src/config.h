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


/* @file config.h
 * This file defines stuff related to the configuration file.
 * It can read customizations on it and set the global variables.
 *
 * If the user specifies options at commandline, they have higher
 * precedence over the config file.
 *
 * Remember, DEFAULT_CONFIG_FILE is defined on the Makefile.
 * By default, it's '/home/<user>/.yetrisrc.ini'
 *
 */

#ifndef CONFIG_H_DEFINED
#define CONFIG_H_DEFINED

#include <stdbool.h>
#include "iniparser/iniparser.h"

void config_handle();
bool config_file_exists(char* filename);
void config_create_default(char* filename);
void config_parse(char* filename);

#endif /* CONFIG_H_DEFINED */


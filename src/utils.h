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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	 If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://yetris.alexdantas.net
 * mailto:	 eu@alexdantas.net
 */

#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <stdbool.h>

/**
 * @file utils.h
 * Here we have random things that may be useful
 * anywhere on the code.
 *
 */

/**
 * Returns a random integer between (including) #min and #max.
 */
int random_int_between(int min, int max);

/**
 * Returns a random #true/#false
 */
bool random_bool();

#endif /* UTILS_H_DEFINED */


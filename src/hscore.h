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
 * homepage: http://www.github.com/alexdantas/yetris/
 * mailto:   alex.dantas92@gmail.com
 */

#ifndef HSCORE_H_DEFINED
#define HSCORE_H_DEFINED

#include "game.h"

#define MAX_HSCORES 10 /* number of hscores to store */

typedef struct score_s
{
	char name[11]; /* 10 chars + null */
	char time[9];  /* hh:mm:ss */
	char date[9];  /* dd/mm/yy */
	int  points;
	int  lines;
	int  level;
} score_s;

/** Top scores list, ordered */
score_s hscores[MAX_HSCORES];

score_s new_score();
void hscore_init();
void hscore_handle(game_s* g);
void score_set(score_s* s, char name[], int points, int lines, int level);
bool is_on_hscore_list(int score);
int hscore_get_highest_points();
int hscore_get_lowest_points();
void hscore_save();
void hscore_load();

#endif /* HSCORE_H_DEFINED */


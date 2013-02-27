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

#include <stdbool.h>
#include "game.h"

/** Maximum number of high scores to store (both on the file and memory) */
#define MAX_HSCORES 10

/** Stores all stuff related to a single score. */
typedef struct score_s
{
	char name[11]; /**< Name of the player (10 chars + null) */
	char time[9];  /**< Time when this score was recorded (hh:mm:ss) */
	char date[9];  /**< Date when this score was recorded (dd/mm/yy) */
//	char timer[9]; /**< Ammount of time the game lasted (hh:mm:ss) */
	int  points;   /**< How many points the player has scored */
	int  lines;    /**< How many lines the player cleared */
	int  level;    /**< On what level the player was when he lost */
} score_s;

/** Global variable storing the top scores list, ordered by points */
score_s hscores[MAX_HSCORES];

score_s new_score();
void hscore_init();
void hscore_handle(game_s* g);
void score_set(score_s* s, char name[], int points, int lines, int level);
bool is_on_hscore_list(int score);
int hscore_get_highest_points();
int hscore_get_lowest_points();
bool hscore_save();
bool hscore_load();
void hscore_reset();

#endif /* HSCORE_H_DEFINED */


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

#define MAX_HSCORES 10 /* number of hscores to store */



	score_s scores[MAX_HSCORES]; /** Top scores list, ordered */


/* score handling stuff  * * * * * * * * * */
typedef struct score_s
{
//	char name[11];
//	char time[9]; /* hh:mm:ss */
//	char date[9]; /* dd/mm/yy */
	int  points;
	int  lines;
	int  level;
} score_s;


void game_score_init(game_s* g);
void game_handle_score(game_s* g);
score_s new_score();
void score_set(score_s* s, char name[], int points, int lines, char time[], int level);
int game_get_hscore_points(game_s* g);


#endif /* HSCORE_H_DEFINED */


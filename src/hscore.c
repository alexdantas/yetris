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

#include <string.h>
#include "hscore.h"
#include <time.h>
/*
1- how can i keep the high score between games?
if i call new_game(), it doesnt zero the hscore and
	it becomes trash. If i do, the hscore itself becomes
	zero.
	I cant seem to pass the previous high score to the next game
*/

/** Starts the high score list with default values  */
void hscore_init()
{
	/* Zeroes all high scores */
	int i;
	for (i = 0; i < MAX_HSCORES; i++)
		hscores[i] = new_score();

	/* My defaults :) */
	strncpy(hscores[0].name, "kure", 8);
	strncpy(hscores[0].date, "24/02/13", 8);
	strncpy(hscores[0].time, "20:13:55", 8);
	hscores[0].points = 1000;
	hscores[0].lines  = 100;
	hscores[0].level  = 10;
}

/** Creates an empty high score element */
score_s new_score()
{
	score_s s;

	memset(s.name, '\0', 11);
	memset(s.date, '\0', 9);
	memset(s.time, '\0', 9);
	s.points = 0;
	s.lines  = 0;
	s.level  = 0;

	return s;
}

/* int get_hscore_index(game_s* g, int score) */
/* { */
/* 	For (i = 10; i > 0; i--) */
/* 	{ */
/* 		if (score < g->scores[i]) */
/* 			return i; */
/* 	} */
/* 	return i; */
/* } */

/* ///////////////////////////////////////////////////////////////////////////
  score handling functions */

void hscore_handle(game_s* g)
{
	char* name = getenv("USER");
	if (!name)
		name = "player";

	if (is_on_hscore_list(g->score))
		score_set(&(hscores[0]), name, g->score, g->lines, g->level);
}

/* int hscore_get_index_of(int score) */
/* { */
/* 	bool bigger = false; */
/* 	int	i; */
/* 	for (i = MAX_HSCORES; i > 0; i--) */
/* 	{ */
/* 		if (bigger) */
/* 			if (points < hscores[i].points) */
/* 				return i; */

/* 		if (points > hscores[i].points) */
/* 			bigger = true; */
/* 	} */
/* 	return i; */
/* } */

bool is_on_hscore_list(int score)
{
	if (score >= hscore_get_lowest_points())
		return true;
	else
		return false;
}

/** Saves a score.
 *  It automagically handles date.
 */
void score_set(score_s* s, char name[], int points, int lines, int level)
{
	strncpy(s->name, name, 8);

	/* Another hack to get the current date and time
	 * Format: Wed Jun 30 21:49:08 1993\n
	 */
	time_t cur_time;
	time(&cur_time);

	strncpy(s->date, "DD/MM/YY", 8);
	strncpy(s->time, (ctime(&cur_time) + 11), 8);

	s->points = points;
	s->lines  = lines;
	s->level  = level;
}

/** Get the points from the first score on the list */
int hscore_get_highest_points()
{
	return hscores[0].points;
}

/** Get the points from the last score on the list */
int hscore_get_lowest_points()
{
	return hscores[MAX_HSCORES - 1].points;
}

void hscore_load()
{
	/* SCORE_PATH defined from Makefile (default /var/games/yetris.scores) */
	FILE* fp = fopen(SCORE_PATH, "rb");
	if (!fp)
	{
		/* score already has defaults */
		hscore_save();
		return;
	}

	/* To ensure portability, I gotta save the version number */
	fread(global.game_version, sizeof(char), 3, fp);
	global.game_version[3] = '\0';

	if (strcmp(global.game_version, VERSION) != 0)
	{
		exit(EXIT_FAILURE);
		// wHAT
	}

	int i;
	for (i = 0; i < MAX_HSCORES; i++)
	{
		fread(hscores[i].name, sizeof(char), 11, fp);
		fread(hscores[i].time, sizeof(char), 9,  fp);
		fread(hscores[i].date, sizeof(char), 9,  fp);

		fread((&hscores[i].points), sizeof(int), 1, fp);
		fread((&hscores[i].lines),  sizeof(int), 1, fp);
		fread((&hscores[i].level),  sizeof(int), 1, fp);
	}
	fclose(fp);
}

void hscore_save()
{
	/* SCORE_PATH defined from Makefile (default /var/games/yetris.scores) */
	FILE* fp = fopen(SCORE_PATH, "wb");
	if (!fp)
		return;

	/* To ensure portability, I gotta save the version number */
	fwrite(VERSION, sizeof(char), 3, fp);

	int i;
	for (i = 0; i < MAX_HSCORES; i++)
	{
		fwrite(hscores[i].name, sizeof(char), 11, fp);
		fwrite(hscores[i].time, sizeof(char), 9,  fp);
		fwrite(hscores[i].date, sizeof(char), 9,  fp);

		fwrite((&hscores[i].points), sizeof(int), 1, fp);
		fwrite((&hscores[i].lines),  sizeof(int), 1, fp);
		fwrite((&hscores[i].level),  sizeof(int), 1, fp);
	}

	fflush(fp);
	fclose(fp);
}


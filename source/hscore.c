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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "hscore.h"
#include "engine.h"


/*
1- how can i keep the high score between games?
if i call new_game(), it doesnt zero the hscore and
	it becomes trash. If i do, the hscore itself becomes
	zero.
	I cant seem to pass the previous high score to the next game
*/

void hscore_handle(game_s* g)
{
	char name[11];
	memset(name, '\0', 11);

	if (getenv("USER") == NULL)
		strncpy(name, "player", 10);
	else
		strncpy(name, getenv("USER"), 10);

	engine_create_input();
	engine_draw_input();
	engine_get_hscore_name(name, 10);
	engine_delete_input();

	if (is_on_hscore_list(g->score))
	{
		score_s s;
		score_set(&s, name, g->score, g->lines, g->level, g->gameplay_h, g->gameplay_m, g->gameplay_s);
		hscore_insert(&s);
	}
}

/** Includes the score #s at it's according position on the global array.
 *  Also shifts any score after the one included.
 */
void hscore_insert(score_s* s)
{
	int index = get_hscore_index(s->points);
	if (index < 0)
		return;

	int i;
	for (i = (MAX_HSCORES - 1); i >= 0; i--)
	{
		if (i == index)
		{
			hscore_copy(&(hscores[i]), s);
			break;
		}
		hscore_copy(&(hscores[i]), &(hscores[i - 1]));
	}
}

/** Returns where the score will be on the global array */
int get_hscore_index(int score)
{
	if (!is_on_hscore_list(score))
		return -1;

	if (score > hscores[0].points)
		return 0;

	int i;
	for (i = (MAX_HSCORES - 1); i > 0; i--)
	{
		if (score >= hscores[i].points)
			if (score < hscores[i - 1].points)
				return i;
	}

	/* what? */
	return -1;
}

/** Starts the high score list with default values  */
void hscore_init()
{
	/* Zeroes all high scores */
	int i;
	for (i = 0; i < MAX_HSCORES; i++)
//		hscores[i] = new_score();
	{
		score_s s = new_score();
		hscore_copy(&(hscores[i]), &s);
	}

	/* My defaults :) */
	strncpy(hscores[0].name,  "kure", 10);
	strncpy(hscores[0].date,  "21/12/2012", 10);
	strncpy(hscores[0].time,  "13:37:00", 8);
	strncpy(hscores[0].timer, "00:06:66", 8);
	hscores[0].points = 5000;
	hscores[0].lines  = 50;
	hscores[0].level  = 5;
}

/** Creates an empty high score element */
score_s new_score()
{
	score_s s;

	memset(s.name,  '\0', 11);
	memset(s.time,  '\0', 9);
	memset(s.date,  '\0', 11);
	memset(s.timer, '\0', 9);
	s.points = 0;
	s.lines  = 0;
	s.level  = 0;

	return s;
}

void hscore_copy(score_s* dest, score_s* orig)
{
	strncpy(dest->name,  orig->name, 11);
	strncpy(dest->time,  orig->time, 9);
	strncpy(dest->date,  orig->date, 11);
	strncpy(dest->timer, orig->timer, 9);
	dest->points = orig->points;
	dest->lines  = orig->lines;
	dest->level  = orig->level;
}

/** Saves a score.
 *  It automagically handles date.
 */
void score_set(score_s* s, char name[], int points, int lines, int level, int hours, int minutes, int seconds)
{
	strncpy(s->name, name, 11);

	/* current gameplay timer - how long the player lasted on the game */
	snprintf(s->timer, 9, "%02d:%02d:%02d", hours, minutes, seconds);

	/* The following stores the date and time on #score
	 * I basically broke down the current time into a struct
	 * with components corresponding to time elements (day, month...).
	 * See 'man localtime' for more info.
	 */
	time_t raw_time = time(NULL);
	struct tm* cur_time = localtime(&(raw_time));
	char tmp_date[11]; memset(tmp_date, '\0', 11);
	char tmp_time[9]; memset(tmp_time, '\0', 9);

	snprintf(tmp_date, 11, "%02d/%02d/%04d",
			  cur_time->tm_mday,
			 (cur_time->tm_mon + 1), /* starts from zero */
			 (cur_time->tm_year) + 1900); /* no of years since 1900 */

	snprintf(tmp_time, 9, "%02d:%02d:%02d",
			  cur_time->tm_hour,
			  cur_time->tm_min,
			  cur_time->tm_sec);

	strncpy(s->date, tmp_date, 10); s->date[10] = '\0';
	strncpy(s->time, tmp_time, 8);

	s->points = points;
	s->lines  = lines;
	s->level  = level;
}

bool is_on_hscore_list(int score)
{
	if (score >= hscore_get_lowest_points())
		return true;
	else
		return false;
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

bool hscore_load()
{
	/* SCORE_PATH defined from Makefile (default /var/games/yetris.scores) */
	FILE* fp = fopen(SCORE_PATH, "rb");
	if (!fp)
	{
		hscore_save(); /* score already has defaults so ill just save */
		return false;
	}

/* This resets the high score file and returns unless fread() everything.
 * This macro receives the same arguments as fread(), It just checks
 * to see if everything has been written just fine.
 */
#define reset_hscore_file_unless_read(what, size, ammount, where)	\
	{                                                               \
		if (fread(what, size, ammount, where) != ammount)           \
		{                                                           \
			hscore_reset();                                         \
			return false;											\
		}                                                           \
	}

	/* The following is to ensure portability.
	 * Every time the game saves the score, it also saves the version
	 * number into the hscore file.
	 * This way, different implementations of the hscore file will
	 * not mess up with each other.
	 * I chose to save the first 3 characters of the version number.
	 * For instance, if the version is 1.51, the game saves the
	 * string "1.5".
	 * I must compare it with the current version and if they're
	 * different, I should convert or reset the hscore file.
	 */
	reset_hscore_file_unless_read(global.game_version, sizeof(char), 3, fp)
	global.game_version[3] = '\0';

	char version_check[4];
	memset(version_check, '\0', 4);
	strncpy(version_check, VERSION, 3);

	if (strcmp(global.game_version, version_check) != 0)
	{
		// should i make a copy and do a backup?
		// exit(EXIT_FAILURE);
		hscore_reset();
		return false;
	}

	int i;
	for (i = 0; i < MAX_HSCORES; i++)
	{
		reset_hscore_file_unless_read(hscores[i].name,  sizeof(char), 11, fp);
		reset_hscore_file_unless_read(hscores[i].time,  sizeof(char), 9,  fp);
		reset_hscore_file_unless_read(hscores[i].date,  sizeof(char), 11,  fp);
		reset_hscore_file_unless_read(hscores[i].timer, sizeof(char), 9,  fp);

		reset_hscore_file_unless_read((&hscores[i].points), sizeof(int), 1, fp);
		reset_hscore_file_unless_read((&hscores[i].lines),  sizeof(int), 1, fp);
		reset_hscore_file_unless_read((&hscores[i].level),  sizeof(int), 1, fp);
	}
	fclose(fp);
	return true;
}

/** Zeroes the contents of the high score file */
void hscore_reset()
{
	FILE* fp = fopen(SCORE_PATH, "wb");
	if (fp)
	{
		fflush(fp);
		fclose(fp);
	}
}

/** Writes the high scores into the file */
bool hscore_save()
{
	/* SCORE_PATH defined from Makefile (default /var/games/yetris.scores) */
	FILE* fp = fopen(SCORE_PATH, "wb");
	if (!fp)
		return false;


/* This returns false unless fwrite() has written everything.
 * This macro receives the same arguments as fwrite(), It just checks
 * to see if everything has been written just fine.
 */
#define return_false_unless_written(what, size, ammount, where)	\
	{                                                           \
		if (fwrite(what, size, ammount, where) != ammount)	    \
			return false;                                       \
	}

	/* To ensure portability, I gotta save the version number */
	return_false_unless_written(VERSION, sizeof(char), 3, fp);

	int i;
	for (i = 0; i < MAX_HSCORES; i++)
	{
		return_false_unless_written(hscores[i].name,  sizeof(char), 11, fp);
		return_false_unless_written(hscores[i].time,  sizeof(char), 9,  fp);
		return_false_unless_written(hscores[i].date,  sizeof(char), 11, fp);
		return_false_unless_written(hscores[i].timer, sizeof(char), 9,  fp);

		return_false_unless_written((&hscores[i].points), sizeof(int), 1, fp);
		return_false_unless_written((&hscores[i].lines),  sizeof(int), 1, fp);
		return_false_unless_written((&hscores[i].level),  sizeof(int), 1, fp);
	}

	// could i just write everything up?
	//	return_false_unless_written(hscores, sizeof(score_s), MAX_HSCORES, fp);

	fflush(fp);
	fclose(fp);
	return true;
}


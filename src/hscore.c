
#include <string.h>
#include "hscore.h"


couldn't make this work because it's very complicated

/*
1- how can i keep the high score between games?
if i call new_game(), it doesnt zero the hscore and
	it becomes trash. If i do, the hscore itself becomes
	zero.
	I cant seem to pass the previous high score to the next game


*/










/** Starts the high score list with default values  */
void game_score_init(game_s* g)
{
	int i;
	for (i = 0; i < MAX_HSCORES; i++)
		g->scores[i] = new_score();
}

bool is_on_hscore_list(game_s* g, int score)
{
	if (score >= game_get_lscore_points(g))
		return true;
	else
		return false;
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

void game_handle_score(game_s* g)
{
	if (is_on_hscore_list(g, g->score))
		score_set(&(g->scores[0]), "kure", g->score, g->lines, "11:02:12", g->level);
	/* int i = get_hscore_index(g, g->score); */
}

score_s new_score()
{
	score_s s;

//	memset(s.name, '\0', 11);
//	memset(s.date, '\0', 9);
//	memset(s.time, '\0', 9);
	s.points = 0;
	s.lines  = 0;
	s.level  = 0;

	return s;
}

/** Saves a score based on current date */
void score_set(score_s* s, char name[], int points, int lines, char time[], int level)
{
//	strncpy(s->name, name, 8);

	//MAKEdate!
//	strncpy(s->date, "06/06/12", 8);

//	strncpy(s->time, "11:99:22", 8);

	s->points = points;
	s->lines  = lines;
	s->level  = level;
}

/** Get the points from the first score on the list */
int game_get_hscore_points(game_s* g)
{
	return g->scores[0].points;
}

/** Get the points from the last score on the list */
int game_get_lscore_points(game_s* g)
{
	return g->scores[MAX_HSCORES - 1].points;
}



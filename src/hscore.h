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


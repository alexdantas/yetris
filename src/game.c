
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "piece.h"
#include "board.h"
#include "engine.h"

/** Initializes and returns a new game structure with all it's dependencies */
game_s new_game()
{
	game_s g;
	int i;

 	srand(time(NULL));
	g.board = new_board();

	g.piece_current = new_piece(piece_get_random());
	for (i = 0; i < NEXT_PIECES_NO; i++)
		g.piece_next[i] = new_piece(piece_get_random());

	g.can_hold  = true;
	g.quit      = false;
	g.is_over   = false;
	g.show_help = false;
	g.is_combo  = false;
	g.moved_piece_down = false;
	g.is_back_to_back  = false;

	g.piece_hold = new_piece(PIECE_DUMMY); /* create a dummy piece */
	game_ghost_update(&g);

	g.score = 0;
	g.lines = 0;
	g.level = 0;
	g.speed = INITIAL_SPEED;
	g.hscore = 0;
	g.combo_count = 0;
	g.back_to_back_count = 0;
	g.back_to_back_lines = 0;
	game_hscore_init(&g);

	g.gameplay_s = 0;
	g.gameplay_m = 0;
	g.gameplay_h = 0;

	timer_start(&(g.piece_timer));
	timer_start(&(g.global_timer));

	return g;
}

/** Refreshes the ghost piece to the current one on #g */
void game_ghost_update(game_s* g)
{
	g->piece_ghost = g->piece_current;
	g->piece_ghost.color = engine_get_color(WHITE_BLACK, false);
	int i;
	for (i = 0; i < 4; i++)
	{
		g->piece_ghost.block[i].type     = EMPTY;
		g->piece_ghost.block[i].color    = g->piece_ghost.color;
		g->piece_ghost.block[i].theme[0] = '[';
		g->piece_ghost.block[i].theme[1] = ']';
		g->piece_ghost.block[i].theme[2] = '\0';
	}
	piece_hard_drop(&(g->piece_ghost), &(g->board));
}

/** Locks the current piece on the board and gets a new one */
void game_lock_piece(game_s* g)
{
	board_lock_piece(&(g->board), &(g->piece_current));

	g->piece_current = g->piece_next[0];
	int i;
	for (i = 0; i < NEXT_PIECES_NO - 1; i++)
		g->piece_next[i] = g->piece_next[i + 1];

	g->piece_next[i] = new_piece(piece_get_random());

	game_ghost_update(g);
	game_delete_possible_lines(g);

	g->can_hold = true; /* now we can switch pieces! */
	g->score += 10;
}

/** Perform any updates on the data structures inside #g */
void game_update(game_s* g)
{
	game_update_piece(g);
	game_update_gameplay_time(g);
	game_update_level(g);
	game_ghost_update(g);

	if (board_is_full(&(g->board)))
		g->is_over = true;
}

/** Updates piece position on screen.
 *
 *  I need the g->moved_piece_down because if the player moved the
 *  piece down, I must reset the piece timer (it wont double-drop it).
 *  But if the player moved the piece sideways, must avoid infinite
 *  floating.
 */
void game_update_piece(game_s* g)
{
	timer_stop(&(g->piece_timer));
	if (timer_delta_mseconds(&(g->piece_timer)) >= g->speed)
	{
		if (!g->moved_piece_down)
		{
			if (piece_can_move(&(g->piece_current), &(g->board), DIR_DOWN))
				piece_move(&(g->piece_current), DIR_DOWN);
			else
				game_lock_piece(g);
		}
		else
		{
			g->moved_piece_down = false;
		}

		timer_start(&(g->piece_timer));
	}
}

/** Updates level and speed based on the number of lines cleared. */
void game_update_level(game_s* g)
{
// OLD WAY, DEPRECATED
//	g->level = g->lines / 15;
//	g->speed = INITIAL_SPEED / (g->level + 1);
//	g->speed = INITIAL_SPEED - ((g->level * 30));

	switch (g->lines)
	{
	case 0:   g->level = 0;  g->speed = 1000; break;
	case 5:   g->level = 1;  g->speed = 950;  break;
	case 10:  g->level = 2;  g->speed = 900;  break;
	case 20:  g->level = 3;  g->speed = 850;  break;
	case 30:  g->level = 4;  g->speed = 800;  break;
	case 50:  g->level = 5;  g->speed = 700;  break;
	case 70:  g->level = 6;  g->speed = 500;  break;
	case 100: g->level = 7;  g->speed = 400;  break;
	case 140: g->level = 8;  g->speed = 300;  break;
	case 170: g->level = 9;  g->speed = 200;  break;
	case 200: g->level = 10; g->speed = 100;  break;
	}
}

/** Saves current piece for later use. If there's already one on the
 *  'hold slot', switch them.
 *  @note Can only do this once per drop.
 */
bool game_hold_piece(game_s* g)
{
	if (!g->can_hold)
		return false;

   	g->can_hold = false;

	piece_s tmp = g->piece_hold;
	g->piece_hold = g->piece_current;

	/* Empty slot - first time holding */
	if (tmp.type == PIECE_DUMMY)
	{
		g->piece_current = g->piece_next[0];
		int i;
		for (i = 0; i < NEXT_PIECES_NO - 1; i++)
			g->piece_next[i] = g->piece_next[i + 1];

		g->piece_next[i] = new_piece(piece_get_random());
		game_ghost_update(g);
	}
	/* All right, switching pieces */
	else
	{
		g->piece_current = tmp;
		piece_reset(&(g->piece_current));
	}

	/* Makes the piece look nice on the hold screen */
	g->piece_hold.rotation = 0;
	int k;
	for (k = 0; k < 4; k++)
	{
		g->piece_hold.block[k].x -= g->piece_hold.x;
		g->piece_hold.block[k].y -= g->piece_hold.y;
	}
	return true;
}

/** Checks all lines, deleting the ones that are full.
 *
 *  @note I know this function's ugly...
 *  @todo Maybe create a 'Line' data structure? To make this simpler?
 */
bool game_delete_possible_lines(game_s* g)
{
	board_s* b = &(g->board);

	bool lines[BOARD_HEIGHT]; /* this will mark lines to be deleted */
	int  count = 0;

	int j;
	for (j = 0; j < BOARD_HEIGHT; j++)
	{
		int i = 0;
		while (i < BOARD_WIDTH)
		{
			if (b->block[i][j].type == EMPTY)
			{
				lines[j] = false;
				break;
			}
			i++;
		}
		if (i == BOARD_WIDTH)
		{
			lines[j] = true;
			count++;
		}
	}
	if (count == 0)
	{
		g->is_combo    = false;
		g->combo_count = 0;
		g->is_back_to_back    = false;
		g->back_to_back_count = 0;
		g->back_to_back_lines = 0;
		return false;
	}

	board_delete_lines(b, lines);
	g->lines += count;

	/* Dealing with score - following the Tetris Guidelines */
	/* Combo Score */
	if (g->is_combo)
	{
		g->combo_count++;
	}
	else
	{
		g->is_combo    = true;
		g->combo_count = 0;
	}

	int combo_score = 0;
	if (g->is_combo)
		combo_score = 50 * g->combo_count * g->level;

	/* Piece Score */
	int piece_score = 0;

	switch (count)
	{
	case 1:  piece_score = 100; break;
	case 2:  piece_score = 300; break;
	case 3:  piece_score = 500; break;
	case 4:  piece_score = 800; break;

	default: piece_score = -1;   break; /* someone's cheating... */
	}

	/* Back-to-Back Lines */
	if (g->is_back_to_back)
	{
		g->back_to_back_count++;
	}
	else
	{
		if ((g->back_to_back_lines == count) && (count >= 2))
		{
			g->is_back_to_back = true;
			g->back_to_back_count = 1;
			piece_score = (piece_score * 3) / 2;
		}
		else
		{
			g->is_back_to_back    = false;
			g->back_to_back_count = false;
		}
	}
	g->back_to_back_lines = count;


	/* Apllying Everything */
	g->score += (piece_score * g->level) + combo_score;

	return true;
}

/* Updates the time indicator on the right screen */
void game_update_gameplay_time(game_s* g)
{
	timer_stop(&(g->global_timer));
	g->gameplay_s = timer_delta_seconds(&(g->global_timer));
	g->gameplay_m = timer_delta_minutes(&(g->global_timer));
	g->gameplay_h = timer_delta_hours(&(g->global_timer));
}

/** Perform actions based on #input.
 *  It must be 'int' (and not 'char') because of ncurses' stuff.
 *
 *  I with it was a big 'switch', but it doesn't work with variables!
 */
void game_handle_input(game_s* g, int input)
{
	if (input == engine.input.quit)
	{
		g->quit = true;
	}
	else if (input == engine.input.left)
 	{
		if (piece_can_move(&(g->piece_current), &(g->board), DIR_LEFT))
		{
			piece_move(&(g->piece_current), DIR_LEFT);
			g->moved_piece_down = false;
		}
	}
	else if (input == engine.input.right)
	{
		if (piece_can_move(&(g->piece_current), &(g->board), DIR_RIGHT))
		{
			piece_move(&(g->piece_current), DIR_RIGHT);
			g->moved_piece_down = false;
		}
	}
	else if (input == engine.input.down)
	{
		if (piece_can_move(&(g->piece_current), &(g->board), DIR_DOWN))
		{
			piece_move(&(g->piece_current), DIR_DOWN);
			g->moved_piece_down = true;
		}
		else
			game_lock_piece(g);
	}
	else if (input == engine.input.rotate)
	{
		if (piece_can_rotate(&(g->piece_current), &(g->board), 1))
			piece_rotate(&(g->piece_current), -1);
	}
	else if (input == engine.input.rotate_backw)
	{
		if (piece_can_rotate(&(g->piece_current), &(g->board), -1))
			piece_rotate(&(g->piece_current), 1);
	}
	else if (input == engine.input.drop)
	{
		piece_hard_drop(&(g->piece_current), &(g->board));
		game_lock_piece(g);
	}
	else if (input == engine.input.pause)
	{
		game_hold_piece(g);
	}
	/* DEBUG KEYS - for development only! */
	else if (input == '+')
	{
		g->level++;
	}
	else if (input == '-')
	{
		g->level--;
	}
	else if (input == KEY_F(2))
	{
		g->show_help = true;
	}
	else if (input == KEY_F(3))
	{
		game_handle_score(g);
	}
	else if (input == KEY_F(4))
	{
		g->gameplay_m++;
	}
}

/** Starts the high score list with default values  */
void game_hscore_init(game_s* g)
{
	FILE* fp = fopen("yetris.hscore", "rb");
	if (!fp)
	{
		g->hscore = 100;
		game_hscore_save(g);
	}
	else
		fread(&(g->hscore), sizeof(g->hscore), 1, fp);
}

void game_over(game_s* g)
{
	timer_stop(&(g->global_timer));
	game_handle_score(g);
}

void game_handle_score(game_s* g)
{
	if (g->score > g->hscore)
	{
		g->hscore = g->score;
		game_hscore_save(g);
	}
}

void game_hscore_save(game_s* g)
{
	FILE* fp = fopen("yetris.hscore", "wb");
	if (fp)
	{
		fwrite(&(g->hscore), sizeof(g->hscore), 1, fp);
		fflush(fp);
	}
}


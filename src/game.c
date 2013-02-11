
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "piece.h"
#include "board.h"
#include "engine.h"

/** Initializes and returns a new game structure with all it's dependencies */
game_s new_game()
{
	game_s g;
	int i;

	g.board = new_board();

	g.piece_current = new_piece(piece_get_random());
	for (i = 0; i < NEXT_PIECES_NO; i++)
		g.piece_next[i] = new_piece(piece_get_random());

	g.can_hold  = true;
	g.quit      = false;
	g.over      = false;
	g.show_help = false;

	g.piece_hold = new_piece(PIECE_DUMMY); /* create a dummy piece */

	g.score = 0;
	g.lines = 0;
	g.level = 0;
	g.speed = INITIAL_SPEED;

	timer_start(&(g.timer));
	game_ghost_update(&g);

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

/** Places the current piece on the board and gets a new one. */
void game_save_piece(game_s* g)
{
	board_save_piece(&(g->board), &(g->piece_current));

	g->piece_current = g->piece_next[0];
	int i;
	for (i = 0; i < NEXT_PIECES_NO - 1; i++)
		g->piece_next[i] = g->piece_next[i + 1];

	g->piece_next[i] = new_piece(piece_get_random());

	game_ghost_update(g);

	g->can_hold = true; /* now we can switch pieces! */
	g->score += 10;
}

/** Perform any updates on the data structures inside #g */
void game_update(game_s* g)
{
	timer_stop(&(g->timer));
	if (timer_delta(&(g->timer)) / 1000 > g->speed)
	{
		if (piece_can_move(&(g->piece_current), &(g->board), DIR_DOWN))
			piece_move(&(g->piece_current), DIR_DOWN);
		else
			game_save_piece(g);

		timer_start(&(g->timer));
	}

	game_delete_possible_lines(g);

	g->level = g->lines / 15;
//	g->speed = INITIAL_SPEED / (g->level + 1);
	g->speed = INITIAL_SPEED - ((g->level * 30));

	game_ghost_update(g);

	if (board_is_full(&(g->board)))
		g->over = true;
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
		return false;

	board_delete_lines(b, lines);
	g->lines += count;

	/* this scoring system follows the tetris guideline */
	switch (count)
	{
	case 1: g->score += (100 * g->level); break;
	case 2: g->score += (300 * g->level); break;
	case 3: g->score += (500 * g->level); break;
	case 4: g->score += (800 * g->level); break;

	default: g->score = 0; break; /* someone's cheating... */
	}
	return true;
}

/* Updates the time indicator on the right screen */
void game_update_time(game_s* g)
{

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
			piece_move(&(g->piece_current), DIR_LEFT);
	}
	else if (input == engine.input.right)
	{
		if (piece_can_move(&(g->piece_current), &(g->board), DIR_RIGHT))
			piece_move(&(g->piece_current), DIR_RIGHT);
	}
	else if (input == engine.input.down)
	{
		if (piece_can_move(&(g->piece_current), &(g->board), DIR_DOWN))
			piece_move(&(g->piece_current), DIR_DOWN);
		else
			game_save_piece(g);
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
		game_save_piece(g);
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

}

/** Starts the high score list with default values  */
void game_hscore_init(game_s* g)
{
	g->hscore = 10;
}

void game_handle_score(game_s* g)
{
	if (g->score > g->hscore)
		g->hscore = g->score;
}


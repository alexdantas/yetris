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
 * homepage: http://www.github.com/alexdantas/yetris/
 * mailto:	 alex.dantas92@gmail.com
 */

#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "piece.h"
#include "board.h"
#include "engine.h"
#include "globals.h"
#include "config.h"
#include "hscore.h"

/* Here's the order of game termination:
 * The player loses (board is full, on update()):
 *
 *	  we call game_over()
 *	  game_over() does one-time settings
 *	  game_over() sets the game state to GAME_OVER
 *	  while the state is GAME_OVER, we wait for a specific keypress
 *		via game_handle_input()
 *	  when we get the keypress we wanted, we set game.is_over to
 *		true, so the main() can see the game has ended.
 *	  main() then restarts the game
 */

/** Initializes and returns a new game structure with all it's dependencies */
game_s new_game()
{
	game_s g;
	int i;

	srand(time(NULL));
	g.state = PLAYING;

	g.board = new_board();
	g.piece_current = new_piece(piece_get_random());

	/* filling next pieces if possible */
	if (global.game_next_no > 0)
		for (i = 0; i < global.game_next_no; i++)
			g.piece_next[i] = new_piece(piece_get_random());

	g.quit		= false;
	g.is_over	= false;
	g.show_help = false;
	g.show_hscores = false;
	g.is_combo	= false;
	g.moved_piece_down = false;
	g.is_back_to_back  = false;
	g.show_score_delta = false;
	g.score_delta = 0;

	if (global.game_can_hold)
	{
		g.can_hold	 = true;
		g.piece_hold = new_piece(PIECE_DUMMY); /* create a dummy piece */
	}

	if (global.game_has_ghost)
		game_ghost_update(&g);

	/* player info */
	g.score		  = 0;
	g.lines		  = 0;
	g.level		  = 1;
	g.speed		  = INITIAL_SPEED;
	g.hscore	  = 0;
	g.combo_count = 0;
	g.back_to_back_count = 0;
	g.back_to_back_lines = 0;
	game_hscore_init(&g);

	/* timer info */
	g.gameplay_s = 0;
	g.gameplay_m = 0;
	g.gameplay_h = 0;
	timer_start(&(g.piece_timer));
	timer_start(&(g.global_timer));

	/* piece statistics */
	g.I_count = 0;
	g.T_count = 0;
	g.L_count = 0;
	g.J_count = 0;
	g.S_count = 0;
	g.Z_count = 0;
	g.O_count = 0;
	g.piece_count = 0;

	/* lines statistics */
	g.single_count = 0;
	g.double_count = 0;
	g.triple_count = 0;
	g.tetris_count = 0;
	g.lines_count  = 0;

	return g;
}

/** Refreshes the ghost piece to the current one on #g */
void game_ghost_update(game_s* g)
{
	g->piece_ghost = g->piece_current;
	g->piece_ghost.color = global.theme_ghost_color;
	g->piece_ghost.theme[0] = global.theme_ghost[0];
	g->piece_ghost.theme[1] = global.theme_ghost[1];

	int i;
	for (i = 0; i < 4; i++)
	{
		g->piece_ghost.block[i].type  = EMPTY;
		g->piece_ghost.block[i].color = g->piece_ghost.color;
		g->piece_ghost.block[i].theme[0] = g->piece_ghost.theme[0];
		g->piece_ghost.block[i].theme[1] = g->piece_ghost.theme[1];
	}
	piece_hard_drop(&(g->piece_ghost), &(g->board));
}

/** Locks the current piece on the board and gets a new one */
void game_lock_piece(game_s* g)
{
	/* before locking, lets add to the statistics */
	if (global.game_has_statistics)
	{
		switch (g->piece_current.type)
		{
		case PIECE_I: g->I_count++; break;
		case PIECE_T: g->T_count++; break;
		case PIECE_L: g->L_count++; break;
		case PIECE_J: g->J_count++; break;
		case PIECE_S: g->S_count++; break;
		case PIECE_Z: g->Z_count++; break;
		case PIECE_O: g->O_count++; break;
		default: break;
		}
		g->piece_count++;
	}

	board_lock_piece(&(g->board), &(g->piece_current));

	g->piece_current = game_get_next_piece(g);
	timer_start(&(g->piece_timer)); /* reset piece falling timer */
	if (global.game_has_ghost)
		game_ghost_update(g);

	game_delete_possible_lines(g);
	game_update_level(g);
	game_update_speed(g);

	/* now we can switch pieces! */
	if (global.game_can_hold)
		g->can_hold = true;

	g->score += 10;
}

/** Returns the next piece and refreshes the piece_next array */
piece_s game_get_next_piece(game_s* g)
{
	if (global.game_next_no == 0)
		return new_piece(piece_get_random());

	piece_s next = g->piece_next[0];
	int i;
	for (i = 0; i < global.game_next_no - 1; i++)
		g->piece_next[i] = g->piece_next[i + 1];

	g->piece_next[i] = new_piece(piece_get_random());
	return next;
}

/** Perform any updates on the data structures inside #g */
void game_update(game_s* g)
{
	switch (g->state)
	{
	case PLAYING:
		game_update_piece(g);
		game_update_gameplay_time(g);

		if (global.game_has_ghost)
			game_ghost_update(g);

		if (board_is_full(&(g->board)))
			game_over(g);

		break;

	/** Will only respond to input */
	case PAUSED:	break;
	case GAME_OVER:	break;
	case HELP:		break;
	case HSCORES:	break;
	case INPUT:		break;

	case QUITTING:
		g->quit = true;
		break;
	}
}

/** Called once when the player loses.
 *	Will set a state GAME_OVER, from where the game should
 *	know how to react.
 *	Usually it respawns itself or sends to the main menu.*/
void game_over(game_s* g)
{
	timer_pause(&(g->global_timer));
	if (global.game_has_game_over_animation)
		engine_draw_gameover_animation(g);

	hscore_handle(g);
	hscore_save();

	g->state = GAME_OVER;
}

/** Updates piece position on screen.
 *
 *	I need the g->moved_piece_down because if the player moved the
 *	piece down, I must reset the piece timer (it wont double-drop it).
 *	But if the player moved the piece sideways, must avoid infinite
 *	floating.
 */
void game_update_piece(game_s* g)
{
	timer_pause(&(g->piece_timer));
	if (timer_delta_ms(&(g->piece_timer)) >= g->speed)
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
	else
		timer_unpause(&(g->piece_timer));
}

/** Updates level and speed based on the number of lines cleared. */
void game_update_level(game_s* g)
{
// OLD WAY, DEPRECATED
//	g->level = g->lines / 15;
//	g->speed = INITIAL_SPEED / (g->level + 1);
//	g->speed = INITIAL_SPEED - ((g->level * 30));

// this is getting too long - need to create a math function
	switch (g->lines)
	{
	case 0:	  g->level = 1;	 break; /* 1000ms */
	case 5:	  g->level = 2;	 break;
	case 10:  g->level = 3;	 break;
	case 15:  g->level = 4;	 break;
	case 20:  g->level = 5;	 break;
	case 25:  g->level = 6;	 break;
	case 30:  g->level = 7;	 break;
	case 40:  g->level = 8;	 break;
	case 50:  g->level = 9;	 break;
	case 60:  g->level = 10; break;
	case 70:  g->level = 11; break;
	case 100: g->level = 12; break;
	case 120: g->level = 13; break;
	case 140: g->level = 14; break;
	case 160: g->level = 15; break;
	case 180: g->level = 16; break;
	case 210: g->level = 17; break;
	case 240: g->level = 18; break;
	}
}

void game_update_speed(game_s* g)
{
	switch (g->level)
	{
	case 1:	 g->speed = INITIAL_SPEED; break; /* 1000ms */
	case 2:	 g->speed = 900;  break;
	case 3:	 g->speed = 850;  break;
	case 4:	 g->speed = 800;  break;
	case 5:	 g->speed = 750;  break;
	case 6:	 g->speed = 700;  break;
	case 7:	 g->speed = 650;  break;
	case 8:	 g->speed = 600;  break;
	case 9:	 g->speed = 550;  break;
	case 10: g->speed = 500;  break;
	case 11: g->speed = 450;  break;
	case 12: g->speed = 400;  break;
	case 13: g->speed = 350;  break;
	case 14: g->speed = 300;  break;
	case 15: g->speed = 250;  break;
	case 16: g->speed = 200;  break;
	case 17: g->speed = 150;  break;
	case 18: g->speed = 100;  break;
	default: g->speed = g->speed; break;
	}
}

/** Saves current piece for later use. If there's already one on the
 *	'hold slot', switch them.
 *	@note Can only do this once per drop.
 */
bool game_hold_piece(game_s* g)
{
	if (!g->can_hold)
		return false;

	g->can_hold = false;

	piece_s tmp	  = g->piece_hold;
	g->piece_hold = new_piece(g->piece_current.type);
	// if we were working with malloc(), we'd free() piece_current now

	/* little hack to pretty-print square pieces */
	if (g->piece_hold.type == PIECE_O)
	{
		int i;
		for (i = 0; i < 4; i++)
			g->piece_hold.block[i].y--;
	}

	/* Empty slot - first time holding */
	if (tmp.type == PIECE_DUMMY)
	{
		g->piece_current = game_get_next_piece(g);
		if (global.game_has_ghost)
			game_ghost_update(g);
	}
	/* All right, switching pieces */
	else
	{
		g->piece_current = new_piece(tmp.type);
		// we'd also free() tmp
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
 *	@note I know this function's ugly...
 *	@todo Maybe create a 'Line' data structure? To make this ?
 *
 *	This function also deals with score. These are the rules:
 *
 *	line_score:			single: 100, double: 300, triple: 500, tetris: 800
 *	combo_score:		50 * combo_count * current_level
 *	back_to_back_score: (line_score * 3) / 2
 *
 *	score: (line_score * current_level) + combo_score
 *	Also, when you hard drop a piece, it's 10 points for free.
 *	Soft drops are the same (although they should be based on height.
 */
bool game_delete_possible_lines(game_s* g)
{
	board_s* b = &(g->board);

	bool lines[BOARD_HEIGHT]; /* this will mark lines to be deleted */
	int	 count = 0; /* how many lines have been cleared */

	g->show_score_delta = false;
	g->score_delta = 0;

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
	/* No lines to be deleted */
	if (count == 0)
	{
		g->is_combo	   = false;
		g->combo_count = 0;
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
		g->is_combo	   = true;
		g->combo_count = 0;
	}

	int combo_score = 0;
	if (g->is_combo)
		combo_score = 50 * g->combo_count * g->level;

	if (g->combo_count > 2)
		g->show_score_delta = true;

	/* Piece Score */
	int piece_score = 0;

	switch (count)
	{
	case 1:
		piece_score = 100;
		g->single_count++;
		break;
	case 2:
		piece_score = 300;
		g->double_count++;
		break;
	case 3:
		piece_score = 500;
		g->triple_count++;
		break;
	case 4:
		piece_score = 800;
		g->tetris_count++;
		break;

	default: piece_score = -1; break; /* someone's cheating... */
	}
	g->lines_count++;

	/* Back-to-Back Lines */
	if ((g->back_to_back_lines == count) && (count >= 2))
	{
		g->back_to_back_count++;
		piece_score = (piece_score * 3) / 2;

		g->show_score_delta = true;
	}
	else
		g->back_to_back_count = 0;

	g->back_to_back_lines = count;

	/* Applying Everything */
	g->score_delta = (piece_score * g->level) + combo_score;
	g->score += g->score_delta;

	return true;
}

/* Updates the time indicator on the right screen */
void game_update_gameplay_time(game_s* g)
{
	timer_pause(&(g->global_timer));
	g->gameplay_s = timer_delta_s(&(g->global_timer));
	g->gameplay_m = timer_delta_m(&(g->global_timer));
	g->gameplay_h = timer_delta_h(&(g->global_timer));
	timer_unpause(&(g->global_timer));
}

/** Perform actions based on #input.
 *	It must be 'int' (and not 'char') because of ncurses' stuff.
 *
 *	I with it was a big 'switch', but it doesn't work with variables!
 */
void game_handle_input(game_s* g, int input)
{
	switch (g->state)
	{
	case PLAYING:
		if (input == engine.input.quit)
		{
			g->state = QUITTING;
		}
		if (input == engine.input.restart)
		{
			game_over(g);
		}
		else if (input == engine.input.left)
		{
			if (piece_move_if_possible(&(g->piece_current), &(g->board), DIR_LEFT))
				g->moved_piece_down = false;
		}
		else if (input == engine.input.right)
		{
			if (piece_move_if_possible(&(g->piece_current), &(g->board), DIR_RIGHT))
				g->moved_piece_down = false;
		}
		else if (input == engine.input.down)
		{
			if (piece_move_if_possible(&(g->piece_current), &(g->board), DIR_DOWN))
				g->moved_piece_down = true;
			else
				game_lock_piece(g);
		}
		else if (input == engine.input.rotate)
		{
			piece_rotate_if_possible(&(g->piece_current), &(g->board), 1);
//			if (piece_can_rotate(&(g->piece_current), &(g->board), 1))
//				piece_rotate(&(g->piece_current), -1);
		}
		else if (input == engine.input.rotate_backw)
		{
			piece_rotate_if_possible(&(g->piece_current), &(g->board), -1);
//			if (piece_can_rotate(&(g->piece_current), &(g->board), -1))
//				piece_rotate(&(g->piece_current), 1);
		}
		else if (input == engine.input.drop)
		{
			piece_hard_drop(&(g->piece_current), &(g->board));
			game_lock_piece(g);
		}
		else if (input == engine.input.pause)
		{
			g->state = PAUSED;
			timer_pause(&(g->global_timer));
		}
		else if (input == engine.input.hold)
		{
			if (global.game_can_hold)
				game_hold_piece(g);
		}
		/* A nice way to alternate statistics! */
		else if (input == KEY_F(2))
		{
			if (global.game_has_statistics)
			{
				global.game_has_statistics = false;
				global.game_has_line_statistics = true;
			}
			else
			{
				global.game_has_statistics = true;
				global.game_has_line_statistics = false;
			}
		}
		/* Refreshing config file! */
		else if (input == KEY_F(5))
		{
			config_handle();
		}
		else if (input == 'h')
		{
			engine_create_help();
			g->state = HELP;
		}
		else if (input == KEY_F(3))
		{
			engine_create_hscores_window();
			g->state = HSCORES;
		}
		else if (input == KEY_F(10))
		{
			game_save(g);
		}
		else if (input == KEY_F(11))
		{
			game_load(g);
		}

/* DEBUG KEYS - for development tests only! */
#ifdef _YETRIS_DEBUG
		else if (input == '+')
		{
			g->level++;
			game_update_speed(g);
		}
		else if (input == '-')
		{
			g->level--;
			game_update_speed(g);
		}
		else if (input == KEY_F(6))
		{
			hscore_handle(g);
		}
		else if (input == KEY_F(7))
		{
			g->gameplay_m++;
		}
		else if (input == KEY_F(8))
		{
			hscore_reset();
		}
#endif
		break;

	case PAUSED:
		if (input == engine.input.quit)
		{
			g->state = QUITTING;
		}
		else if (input == engine.input.pause)
		{
			g->state = PLAYING;
			timer_unpause(&(g->global_timer));
		}
		else if (input == KEY_F(2))
		{
			game_switch_statistics();
		}
		break;

	case GAME_OVER:
		if (input == engine.input.quit)
		{
			g->state = QUITTING;
		}
		else if (input == '\n')
		{
			g->is_over = true; /* warns main() to restart the game */
		}
		else if (input == KEY_F(2))
		{
			if (global.game_has_statistics)
			{
				global.game_has_statistics = false;
				global.game_has_line_statistics = true;
			}
			else
			{
				global.game_has_statistics = true;
				global.game_has_line_statistics = false;
			}
		}
		break;

	case HELP:
		if (input == '\n')
		{
			engine_delete_help(g);
			g->state = PLAYING;
			engine_draw(g);
		}
		else if (input == engine.input.quit)
		{
			g->state = QUITTING;
		}
		break;

	case HSCORES:
		if (input == '\n')
		{
			engine_delete_hscores_window(g);
			g->state = PLAYING;
			engine_draw(g);
		}
		else if (input == engine.input.quit)
		{
			g->state = QUITTING;
		}
		break;

	case INPUT:
		if (input == engine.input.quit)
		{
			g->state = QUITTING;
		}
		break;

	default: /* Welp... Do nothing, I guess... */ break;
	}
}

/** Starts the high score list with default values	*/
void game_hscore_init(game_s* g)
{
	FILE* fp = fopen(global.hscore_filename, "rb");
	if (!fp)
	{
		g->hscore = 100;
		game_hscore_save(g);
	}
	else
		fread(&(g->hscore), sizeof(g->hscore), 1, fp);
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
	FILE* fp = fopen(global.hscore_filename, "wb");
	if (fp)
	{
		fwrite(&(g->hscore), sizeof(g->hscore), 1, fp);
		fflush(fp);
	}
}

/** Switches what statistics appear on the info screen. */
void game_switch_statistics()
{
	if (global.game_has_statistics)
	{
		global.game_has_statistics = false;
		global.game_has_line_statistics = true;
	}
	else
	{
		global.game_has_statistics = true;
		global.game_has_line_statistics = false;
	}
}


/*	*/
/*	*/
/* experimental functions that saves the game state */
/*	*/
/*	*/

#define SAVE_PATH "yetris01.sav"

/** Saves the current game state.
 *	Interrupts if something bad happens.
 */
void game_save(game_s* g)
{
	FILE* fp = fopen(SAVE_PATH, "wb");
	if (!fp)
		return;

/* This returns unless fwrite() has written everything.
 * This macro receives the same arguments as fwrite(), It just checks
 * to see if everything has been written just fine.
 */
#define return_unless_written(what, size, ammount, where)		\
	{															\
		if (fwrite(what, size, ammount, where) != ammount)		\
			return;												\
	}

	return_unless_written(VERSION,			   sizeof(char),	3, fp);
	return_unless_written(&(g->piece_current), sizeof(piece_s), 1, fp);
	return_unless_written(&(g->piece_next),	   sizeof(piece_s), NEXT_PIECES_NO, fp);
	return_unless_written(&(g->piece_ghost),   sizeof(piece_s), 1, fp);
	return_unless_written(&(g->piece_hold),	   sizeof(piece_s), 1, fp);
	return_unless_written(&(g->board),		   sizeof(board_s), 1, fp);

	return_unless_written(&(g->global_timer),  sizeof(timer_s), 1, fp);
	return_unless_written(&(g->piece_timer),   sizeof(timer_s), 1, fp);

	return_unless_written(&(g->gameplay_s),	   sizeof(long), 1, fp);
	return_unless_written(&(g->gameplay_m),	   sizeof(long), 1, fp);
	return_unless_written(&(g->gameplay_h),	   sizeof(long), 1, fp);

	return_unless_written(&(g->piece_hold),	   sizeof(piece_s), 1, fp);

	return_unless_written(&(g->score), sizeof(int), 1, fp);
	return_unless_written(&(g->lines), sizeof(int), 1, fp);
	return_unless_written(&(g->level), sizeof(int), 1, fp);
	return_unless_written(&(g->speed), sizeof(int), 1, fp);
	return_unless_written(&(g->hscore), sizeof(int), 1, fp);
	return_unless_written(&(g->combo_count), sizeof(int), 1, fp);
	return_unless_written(&(g->back_to_back_lines), sizeof(int), 1, fp);
	return_unless_written(&(g->back_to_back_count), sizeof(int), 1, fp);
	return_unless_written(&(g->score_delta), sizeof(int), 1, fp);

	return_unless_written(&(g->can_hold), sizeof(bool), 1, fp);
	return_unless_written(&(g->quit), sizeof(bool), 1, fp);
	return_unless_written(&(g->is_over), sizeof(bool), 1, fp);
	return_unless_written(&(g->show_help), sizeof(bool), 1, fp);
	return_unless_written(&(g->show_hscores), sizeof(bool), 1, fp);
	return_unless_written(&(g->moved_piece_down), sizeof(bool), 1, fp);
	return_unless_written(&(g->is_combo), sizeof(bool), 1, fp);
	return_unless_written(&(g->is_back_to_back), sizeof(bool), 1, fp);
	return_unless_written(&(g->show_score_delta), sizeof(bool), 1, fp);

	return_unless_written(&(g->state), sizeof(game_state), 1, fp);

	return_unless_written(&(g->I_count), sizeof(int), 1, fp);
	return_unless_written(&(g->T_count), sizeof(int), 1, fp);
	return_unless_written(&(g->L_count), sizeof(int), 1, fp);
	return_unless_written(&(g->J_count), sizeof(int), 1, fp);
	return_unless_written(&(g->S_count), sizeof(int), 1, fp);
	return_unless_written(&(g->Z_count), sizeof(int), 1, fp);
	return_unless_written(&(g->O_count), sizeof(int), 1, fp);
	return_unless_written(&(g->piece_count), sizeof(int), 1, fp);

	return_unless_written(&(g->single_count), sizeof(int), 1, fp);
	return_unless_written(&(g->double_count), sizeof(int), 1, fp);
	return_unless_written(&(g->triple_count), sizeof(int), 1, fp);
	return_unless_written(&(g->tetris_count), sizeof(int), 1, fp);
	return_unless_written(&(g->lines_count), sizeof(int), 1, fp);

	return_unless_written(&global, sizeof(globals_s), 1, fp);
}

/** Loads a saved game state.
 *	Interrupts if something wrong happens.
 */
void game_load(game_s* g)
{
	FILE* fp = fopen(SAVE_PATH, "rb");
	if (!fp)
		return;

/* This returns unless fread() everything.
 * This macro receives the same arguments as fread(), It just checks
 * to see if everything has been written just fine.
 */
#define return_unless_read(what, size, ammount, where)				\
	{																\
		if (fread(what, size, ammount, where) != ammount)			\
			return;													\
	}

	char version[4];
	return_unless_read(version, sizeof(char), 3, fp)
	version[3] = '\0';

	char version_check[4];
	memset(version_check, '\0', 4);
	strncpy(version_check, VERSION, 3);

	if (strcmp(version, version_check) != 0)
		return;

	return_unless_read(&(g->piece_current), sizeof(piece_s), 1, fp);
	return_unless_read(&(g->piece_next),	sizeof(piece_s), NEXT_PIECES_NO, fp);
	return_unless_read(&(g->piece_ghost),	sizeof(piece_s), 1, fp);
	return_unless_read(&(g->piece_hold),	sizeof(piece_s), 1, fp);
	return_unless_read(&(g->board),			sizeof(board_s), 1, fp);

	return_unless_read(&(g->global_timer),	sizeof(timer_s), 1, fp);
	return_unless_read(&(g->piece_timer),	sizeof(timer_s), 1, fp);

	return_unless_read(&(g->gameplay_s),	sizeof(long), 1, fp);
	return_unless_read(&(g->gameplay_m),	sizeof(long), 1, fp);
	return_unless_read(&(g->gameplay_h),	sizeof(long), 1, fp);

	return_unless_read(&(g->piece_hold),	sizeof(piece_s), 1, fp);

	return_unless_read(&(g->score), sizeof(int), 1, fp);
	return_unless_read(&(g->lines), sizeof(int), 1, fp);
	return_unless_read(&(g->level), sizeof(int), 1, fp);
	return_unless_read(&(g->speed), sizeof(int), 1, fp);
	return_unless_read(&(g->hscore), sizeof(int), 1, fp);
	return_unless_read(&(g->combo_count), sizeof(int), 1, fp);
	return_unless_read(&(g->back_to_back_lines), sizeof(int), 1, fp);
	return_unless_read(&(g->back_to_back_count), sizeof(int), 1, fp);
	return_unless_read(&(g->score_delta), sizeof(int), 1, fp);

	return_unless_read(&(g->can_hold), sizeof(bool), 1, fp);
	return_unless_read(&(g->quit), sizeof(bool), 1, fp);
	return_unless_read(&(g->is_over), sizeof(bool), 1, fp);
	return_unless_read(&(g->show_help), sizeof(bool), 1, fp);
	return_unless_read(&(g->show_hscores), sizeof(bool), 1, fp);
	return_unless_read(&(g->moved_piece_down), sizeof(bool), 1, fp);
	return_unless_read(&(g->is_combo), sizeof(bool), 1, fp);
	return_unless_read(&(g->is_back_to_back), sizeof(bool), 1, fp);
	return_unless_read(&(g->show_score_delta), sizeof(bool), 1, fp);

	return_unless_read(&(g->state), sizeof(game_state), 1, fp);

	return_unless_read(&(g->I_count), sizeof(int), 1, fp);
	return_unless_read(&(g->T_count), sizeof(int), 1, fp);
	return_unless_read(&(g->L_count), sizeof(int), 1, fp);
	return_unless_read(&(g->J_count), sizeof(int), 1, fp);
	return_unless_read(&(g->S_count), sizeof(int), 1, fp);
	return_unless_read(&(g->Z_count), sizeof(int), 1, fp);
	return_unless_read(&(g->O_count), sizeof(int), 1, fp);
	return_unless_read(&(g->piece_count), sizeof(int), 1, fp);

	return_unless_read(&(g->single_count), sizeof(int), 1, fp);
	return_unless_read(&(g->double_count), sizeof(int), 1, fp);
	return_unless_read(&(g->triple_count), sizeof(int), 1, fp);
	return_unless_read(&(g->tetris_count), sizeof(int), 1, fp);
	return_unless_read(&(g->lines_count), sizeof(int), 1, fp);

	return_unless_read(&global, sizeof(globals_s), 1, fp);
}


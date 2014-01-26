
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <sys/time.h>  /* select() */
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include "engine.h"
#include "game.h"
#include "globals.h"
#include "hscore.h"
#include "piece.h"
#include "piece_definitions.h"

/* Windows can't handle 'stdin' or 'stdout'.
 * So for now i must keep this */
#if OS_IS_WINDOWS
#define fprintf(stderr, printf(
#endif

/**
 * Local function.
 *
 * Whenever we get a signal from the player or the system,
 * we're calling engine_safe_exit to safely exit ncurses
 */
void register_signal_handler();

bool engine_init()
{
	/* signals during initialization */
	block_signals();

	/* Starting ncurses! */
	initscr();

	if (global.screen_use_colors)
		color_init();

	cbreak();    /* Character input doesnt require the <enter> key anymore */
	curs_set(0); /* Makes the blinking cursor invisible */
	noecho();    /* Wont print the keys received through input */
	nodelay(stdscr, TRUE); /* Wont wait for input */
	keypad(stdscr, TRUE);  /* Support for extra keys (life F1, F2, ... ) */
	refresh();   /* Refresh the layout (prints whats in the layout buffer) */

	restore_signals();
	register_signal_handler();
	return true;
}

void register_signal_handler()
{
	struct sigaction sigint_handler;
	sigint_handler.sa_handler = engine_safe_exit;

	sigaction(SIGHUP,  &sigint_handler, NULL);
	sigaction(SIGINT,  &sigint_handler, NULL);
	sigaction(SIGQUIT, &sigint_handler, NULL);
	sigaction(SIGILL,  &sigint_handler, NULL);
	sigaction(SIGABRT, &sigint_handler, NULL);
	sigaction(SIGFPE,  &sigint_handler, NULL);
	sigaction(SIGSEGV, &sigint_handler, NULL);
	sigaction(SIGTERM, &sigint_handler, NULL);
}

bool block_signals()
{
	struct sigaction sigint_handler;
	sigint_handler.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sigint_handler, NULL);
	return true;
}

bool restore_signals()
{
	struct sigaction sigint_handler;
	sigint_handler.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sigint_handler, NULL);
	return true;
}

void engine_exit()
{
	erase();
	refresh();
	endwin();
}

void engine_safe_exit(int sig)
{
	engine_exit();

	/* since the game doesn't deal with malloc (yet) we're
	 * pretty much safe quitting like this.
	 * I've already set that when we call exit() we quit
	 * from the engine, anyway.
	 */
	fprintf(stderr, "Interrupted (signal %d).\n"
	        "Bad game, no donut for you!\n", sig);

	/* won't call functions registered with atexit() */
	_exit(EXIT_FAILURE);
}

int engine_keymap(char keymap[])
{
	/* TODO: check for repeated letters */

	/* ncurses' constants */
	engine.input.none = ERR;

	if (! keymap ||
	    strnlen(keymap, NUMBER_OF_KEYS + 1) != NUMBER_OF_KEYS)
	{
		/* Invalid string, setting default keymap */
		engine.input.down         = KEY_DOWN;
		engine.input.right        = KEY_RIGHT;
		engine.input.left         = KEY_LEFT;
		engine.input.rotate       = 'x';
		engine.input.rotate_backw = 'z';
		engine.input.drop         = ' ';
		engine.input.hold         = 'c';
		engine.input.pause        = 'p';
		engine.input.quit         = 'q';
		engine.input.restart      = 'r';
		return -1;
	}

	engine.input.down         = keymap[0];
	engine.input.right        = keymap[1];
	engine.input.left         = keymap[2];
	engine.input.rotate       = keymap[3];
	engine.input.rotate_backw = keymap[4];
	engine.input.drop         = keymap[5];
	engine.input.pause        = keymap[6];
	engine.input.quit         = keymap[7];
	engine.input.hold         = keymap[8];
	engine.input.restart      = keymap[8];
	return 0;
}


int engine_get_input(int delay_ms)
{
	int retval = 0;
	int c      = 0;
	fd_set input;
	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = delay_ms * 1000; /* microseconds */

	FD_ZERO(&input);
	FD_SET(STDIN_FILENO, &input);

	/* This function is somewhat complex - check 'man select' for info */
	retval = select(FD_SETSIZE, &input, NULL, NULL, &timeout);
	c = getch();

	if ((retval == 1) && (c == ERR)) /* ERROR */
		return -1;

	if (retval == 0)
		return engine.input.none;

	return c;
}

void engine_draw_piece(piece_s* p, WINDOW* w)
{
	if (!piece_is_valid(p))
		return;

	int i, j;

	for (i = 0; i < PIECE_BLOCKS; i++)
		for (j = 0; j < PIECE_BLOCKS; j++)

			if (global_pieces[p->type][p->rotation][j][i] != 0)
				engine_draw_block_theme(w,
				                        p->theme,
				                        (p->x + i) * 2,
				                        p->y + j);
}

void engine_draw_board(board_s* b)
{
	WINDOW* w = engine.layout.board.win;

	int i, j;
	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			engine_draw_block_theme(w,
			                        b->block[i][j],
			                        (b->x + i) * 2, /* 2 chars */
			                         b->y + j);

	if (global.screen_fancy_borders)
		window_fancy_borders(engine.layout.middle_left.win);
	else
		window_normal_borders(engine.layout.middle_left.win);

	wnoutrefresh(engine.layout.middle_left.win);
}

void engine_draw_block_theme(WINDOW* w, block_theme_s* t, int x, int y)
{
	if (!w || !t)
		return;

	if (global.screen_use_colors)
		color_pair_activate(w, t->color);
	else
		color_pair_activate(w, global.theme_piece_colorless.color);

	mvwaddch(w, y, x,     t->appearance[0]);
	mvwaddch(w, y, x + 1, t->appearance[1]);
}


void engine_draw_pause()
{
	window_s* w = &(engine.layout.board);

	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w->win, w->height/2 - 1, w->width/2 - 4, "[paused]");
	wnoutrefresh(w->win);
}

/* LOCAL FUNCTION WTF */

void engine_draw_next_pieces(game_s* g)
{
	WINDOW* w = NULL;
	int i;
	for (i = 0; i < global.game_next_no; i++)
	{
		piece_s p = g->piece_next[i];
		w = engine.layout.next[i].win;

		werase(w);

		/* This is a little hack to pretty-print pieces
		 * TODO somehow manage to fix this */
		p.x = -1;
		p.y = -1;

		if (p.type == PIECE_O)
			p.y -= 1;

		engine_draw_piece(&p, w);
		wnoutrefresh(w);
	}

	w = engine.layout.middle_right.win;

	if (global.screen_fancy_borders)
	{
		mvwaddch(w, 3, 0, ACS_LLCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
		mvwhline(w, 3, 1, ACS_HLINE|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD, 8);
		mvwaddch(w, 3, 9, ACS_LRCORNER|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD);
	}
	else
	{
		wattrset(w, color_pair(COLOR_BLACK, COLOR_DEFAULT, true));
		mvwhline(w, 3, 1, '-', 8);
	}

	wattrset(w, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w, 0, 1, "Next");
	wnoutrefresh(w);

	window_s* win = &(engine.layout.middle_right);

	/* RE-DRAWING BORDERS (damn this sucks) */
	if (global.screen_fancy_borders)
	{
		window_fancy_borders(win->win);
		/* making the top line between 1st next and the rest */
		mvwaddch(win->win, 3, 0, ACS_LLCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
		mvwhline(win->win, 3, 1, ACS_HLINE|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD, win->width - 2);
		mvwaddch(win->win, 3, win->width - 1, ACS_LRCORNER|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD);

		/* making the bottom line between 1st next and the rest */
		mvwaddch(win->win, 4, 0, ACS_ULCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false)|A_BOLD);
		mvwhline(win->win, 4, 1, ACS_HLINE|color_pair(COLOR_WHITE, COLOR_DEFAULT, false), win->width - 2);
		mvwaddch(win->win, 4, win->width - 1, ACS_URCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));

	}
	else
	{
		window_normal_borders(win->win);
		wattrset(win->win, color_pair(COLOR_BLACK, COLOR_DEFAULT, true));
		mvwhline(win->win, 3, 1, '-', win->width - 2);
	}

}

void engine_draw_hold(game_s* g)
{
	window_s* w = NULL;
	piece_s*  p = &(g->piece_hold);

	w = &(engine.layout.leftmost);
	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w->win, 0, 1, "Hold");
	wnoutrefresh(w->win);

	w = &(engine.layout.hold);
	werase(w->win);
	engine_draw_piece(p, w->win);
	wnoutrefresh(w->win);

	w = &(engine.layout.leftmost);

	/* DRAWING BORDERS, AGGGHW */
	if (global.screen_fancy_borders)
	{
		window_fancy_borders(w->win);

		/* If the player has no hold, doesnt make sense printing these parts */
		if (global.game_can_hold)
		{
			/* making the top line between hold and score windows */
			mvwaddch(w->win, 5, 0, ACS_LLCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
			my_mvwhline(w->win, 5, 1, ACS_HLINE|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD, w->width - 2);
			mvwaddch(w->win, 5, w->width - 1, ACS_LRCORNER|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD);

			/* making the bottom line between hold and score windows */
			mvwaddch(w->win, 6, 0, ACS_ULCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false)|A_BOLD);
			my_mvwhline(w->win, 6, 1, ACS_HLINE|color_pair(COLOR_WHITE, COLOR_DEFAULT, false), w->width - 2);
			mvwaddch(w->win, 6, w->width - 1, ACS_URCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
		}

	}
	else
	{
		window_normal_borders(w->win);
		wattrset(w->win, color_pair(COLOR_BLACK, COLOR_DEFAULT, true));
		mvwhline(w->win, 5, 1, '-', w->width - 2);
	}
}

void engine_draw_score(game_s* g)
{
	window_s w = engine.layout.score;

	werase(w.win);

	/* If user has combo, let's show it to him */
	if ((g->is_combo) && (g->combo_count > 0))
	{
		if (g->combo_count > 3)
			wattrset(w.win, color_pair(COLOR_RED, COLOR_DEFAULT, false));
		else
			wattrset(w.win, color_pair(COLOR_YELLOW, COLOR_DEFAULT, false));

		mvwaddstr(w.win, 0, 2, "Combo!");
		mvwprintw(w.win, 0, 8, "x%d", g->combo_count);
	}

	/* If user has back-to-back lines, let's show it to him */
	if (g->back_to_back_count > 0)
	{
		if (g->back_to_back_lines < 4)
			wattrset(w.win, color_pair(COLOR_RED,    COLOR_BLACK, true));
		else
			wattrset(w.win, color_pair(COLOR_YELLOW, COLOR_DEFAULT, true));

		mvwaddstr(w.win, 1, 0, "Back-to-back");
		switch (g->back_to_back_lines)
		{
		case 2: mvwaddstr(w.win, 2, 2, "Double"); break;
		case 3: mvwaddstr(w.win, 2, 2, "Triple"); break;
		case 4: mvwaddstr(w.win, 2, 2, "Tetris"); break;
		}
		if (g->back_to_back_count > 1)
			mvwprintw(w.win, 2, 8, "x%d", g->back_to_back_count);
	}

	wattrset(w.win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w.win, 3,  1, "High Score");
	mvwaddstr(w.win, 6,  1, "Score");
	mvwaddstr(w.win, 9,  1, "Lines");
	mvwaddstr(w.win, 12, 1, "Level");

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, 4,  1, "%10d", hscores[0].points);
	mvwprintw(w.win, 7,  1, "%10d", g->score);
	mvwprintw(w.win, 10, 1, "%10d", g->lines);
	mvwprintw(w.win, 13, 9, "%02d", g->level);

	if (g->show_score_delta)
	{
		wattrset(w.win, color_pair(COLOR_GREEN, COLOR_DEFAULT, true));
		mvwprintw(w.win,  8, 1, "%10d", g->score_delta);
	}

	wnoutrefresh(w.win);
}

void engine_draw_statistics(game_s* g)
{
	window_s w = engine.layout.info;
	int x_offset = 6;
	int y_offset = 1;

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, 3, 1, "%10d x", g->I_count);
	piece_s p = new_piece(PIECE_I);
	p.x = x_offset;
	p.y = y_offset;
	engine_draw_piece(&p, w.win);

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, 5, 1, "%10d x", g->T_count);
	p = new_piece(PIECE_T);
	p.x = x_offset;
	p.y = y_offset + 2;
	engine_draw_piece(&p, w.win);

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, 7, 1, "%10d x", g->L_count);
	p = new_piece(PIECE_L);
	p.x = x_offset;
	p.y = y_offset + 4;
	engine_draw_piece(&p, w.win);

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, 9, 1, "%10d x", g->J_count);
	p = new_piece(PIECE_J);
	p.x = x_offset;
	p.y = y_offset + 6;
	engine_draw_piece(&p, w.win);

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, 11, 1, "%10d x", g->S_count);
	p = new_piece(PIECE_S);
	p.x = x_offset;
	p.y = y_offset + 8;
	engine_draw_piece(&p, w.win);

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, 13, 1, "%10d x", g->Z_count);
	p = new_piece(PIECE_Z);
	p.x = x_offset;
	p.y = y_offset + 10;
	engine_draw_piece(&p, w.win);

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, 15, 1, "%10d x", g->O_count);
	p = new_piece(PIECE_O);
	p.x = x_offset - 1;
	p.y = y_offset + 11;
	engine_draw_piece(&p, w.win);

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, 16, 1, "%10d   Total", g->piece_count);
}

void engine_draw_line_statistics(game_s* g)
{
	window_s* w = &(engine.layout.info);

	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, true));
	mvwaddstr(w->win, 3, 1, "Single:");
	mvwaddstr(w->win, 4, 1, "Double:");
	mvwaddstr(w->win, 5, 1, "Triple:");
	mvwaddstr(w->win, 6, 1, "Tetris:");
	mvwaddstr(w->win, 7, 1, "Total:");

	wattrset(w->win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w->win, 3, 9, "%10d", g->single_count);
	mvwprintw(w->win, 4, 9, "%10d", g->double_count);
	mvwprintw(w->win, 5, 9, "%10d", g->triple_count);
	mvwprintw(w->win, 6, 9, "%10d", g->tetris_count);
	mvwprintw(w->win, 7, 9, "%10d", g->lines_count);
}

/**
 * Draws everything that's on the info
 * window (the rightmost one).
 *
 * WTF LOCAL FUNCTION
 */
void engine_draw_info(game_s* g)
{
	window_s w = engine.layout.info;

	werase(w.win);

	/* Can only draw one at a time D: */
	if (global.game_has_statistics)
		engine_draw_statistics(g);

	else if (global.game_has_line_statistics)
		engine_draw_line_statistics(g);

	wattrset(w.win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w.win, 0, 0, "yetris v"VERSION);
	mvwaddstr(w.win, 1, 1, "(press 'h' for info)");
	mvwaddstr(w.win, w.height - 1, 0, "Timer:");

	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	if (g->gameplay_h) /* Wow.. will someone really play this game for hours? */
		mvwprintw(w.win, w.height - 1, 7, "%02d:%02d:%02d", g->gameplay_h, g->gameplay_m % 60, g->gameplay_s % 60);
	else
		mvwprintw(w.win, w.height - 1, 7, "%02d:%02d", g->gameplay_m % 60, g->gameplay_s % 60);

	wattrset(w.win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w.win, w.height - 2, 0, "Speed:");
	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwprintw(w.win, w.height - 2, 7, "%dms", g->speed);

	/* This is a little hack to display the time onlayout.
	 * It's ugly as hell, but I had to make it
	 * Format: Wed Jun 30 21:49:08 1993\n */
	time_t cur_time;
	time(&cur_time);
	wattrset(w.win, color_pair(COLOR_BLACK, COLOR_DEFAULT, true));
	mvwprintw(w.win, w.height - 1, 15, "%.8s", (ctime(&cur_time) + 11));

	/* Showing FPS */
//  mvwprintw(w.win, w.height - 2, 15, "FPS: %d", global.fps);

	/* DRAW BORDERS AGGHWW */
	w = engine.layout.rightmost;
	if (global.screen_fancy_borders)
		window_fancy_borders(w.win);
	else
		window_normal_borders(w.win);

	wnoutrefresh(w.win);
}

void engine_draw(game_s* g)
{
	WINDOW* board = engine.layout.board.win;
	werase(board);

	switch (g->state)
	{
	case PLAYING:
		engine_draw_board(&(g->board));
		engine_draw_piece(&(g->piece_ghost), board);
		engine_draw_piece(&(g->piece_current), board);
		if (global.game_next_no > 0)
			engine_draw_next_pieces(g);
		if (global.game_can_hold)
			engine_draw_hold(g);
		engine_draw_score(g);
		engine_draw_info(g);
		break;

	case PAUSED:
		engine_draw_board(&(g->board));
		engine_draw_pause();
		engine_draw_info(g); /* refresh the current time */
		break;

	case GAME_OVER:
		engine_draw_board(&(g->board));
		engine_draw_gameover();
		engine_draw_info(g);
		break;

	case HELP:
		engine_draw_board(&(g->board));
		engine_draw_info(g);
		engine_draw_help();
		break;

	case HSCORES:
		engine_draw_board(&(g->board));
		engine_draw_info(g);
		engine_draw_hscores();
		break;

	default: /* Umm... Nothing, I guess...? */ break;
	}
	wnoutrefresh(board);
	doupdate();
}

void engine_draw_gameover_animation(game_s* g)
{
	board_s*  b = &(g->board);
	window_s* w = &(engine.layout.board);

	int i, j;
	for (j = 0; j < BOARD_HEIGHT; j++)
	{
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			if (b->block[i][j])
			{
				b->block[i][j]->color = color_pair(COLOR_WHITE, COLOR_WHITE, true);
				engine_draw_block_theme(w->win,
				                        b->block[i][j],
				                        (b->x + i) * 2,
				                        b->y + j);
			}
		}
		/* Stop a little (50ms) before painting next line */
		usleep(50000);
		wrefresh(w->win);
	}
	wrefresh(w->win);
	/* Now wait a second, to let the feeling of defeat sink in */
//  usleep(1000000);
}

void engine_draw_gameover()
{
	window_s* w = &(engine.layout.board);

	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, true));
	mvwaddstr(w->win, w->height/2 - 1, w->width/2 - 4, "Game Over");
	mvwaddstr(w->win, w->height/2 + 1, 4,              "Press <Enter>");
	mvwaddstr(w->win, w->height/2 + 2, 5,              "to restart");
	wnoutrefresh(w->win);
}

void engine_refresh_all_windows()
{
	layout_s* s = &(engine.layout);

// NOT USING THIS MACRO NOW
#define fancy_borders_and_refresh(window)	  \
	{ \
		if (global.screen_fancy_borders) \
			window_fancy_borders(window); \
		else \
			window_normal_borders(window); \
		wnoutrefresh(window); \
	}
	wnoutrefresh(stdscr);
	wnoutrefresh(s->main.win);

	// FOR NOW IT DELETES AND RECREATES THEM
	// IS THERE A BETTER WAY?
	delwin(s->main.win);
	delwin(s->leftmost.win);
	delwin(s->middle_left.win);
	delwin(s->middle_right.win);
	delwin(s->rightmost.win);

	layout_windows_init();
}

void engine_create_help()
{
	window_s w;

	w.width  = engine.layout.main.width - engine.layout.main.width/8;
	w.height = engine.layout.main.height/2 + 5;
	w.x      = engine.layout.main.width/2 - w.width/2 /* center */;
	w.y      = engine.layout.main.height/2 - w.height/2;
	w.win    = derwin(engine.layout.main.win, w.height, w.width, w.y, w.x);

	if (global.screen_fancy_borders)
		window_fancy_borders(w.win);
	else
		window_normal_borders(w.win);

	engine.layout.help_container = w;

	w.width  = engine.layout.help_container.width - 2;
	w.height = engine.layout.help_container.height - 2;
	w.x      = 1;
	w.y      = 1;
	w.win    = derwin(engine.layout.help_container.win, w.height, w.width, w.y, w.x);
	engine.layout.help = w;
}

void engine_draw_help()
{
	window_s* w = NULL;

	w = &(engine.layout.help_container);
	if (global.screen_fancy_borders)
		window_fancy_borders(w->win);
	else
		window_normal_borders(w->win);

	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w->win, 0, 1, "Help");
	wnoutrefresh(w->win);

	w = &(engine.layout.help);
	werase(w->win);

	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, true));
	mvwaddstr(w->win, 0, 2, "Controls:\n");
	wattrset(w->win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	mvwaddstr(w->win, 1, 0,
	          "    Enter        Return to the game\n"
	          "    q            Quits game at any time\n"
	          "    Left, Right  Controls the piece\n"
	          "    Down         Soft-drop\n"
	          "    Space        Hard-drop\n"
	          "    c            Holds the piece\n"
	          "    z, x         Rotates piece counter-clockwise and clockwise\n"
	          "    p            Pauses/unpauses the game\n"
	          "    r            Restart game\n"
	          "    F2           Switch statistics\n"
	          "    F3           Show high scores\n"
	          "    F5           Refresh game based on config file\n");

	mvwaddstr(w->win,w->height - 1, 0, "For fun, check config file '~/.yetrisrc.ini'");
	wnoutrefresh(w->win);
}

void engine_create_hscores_window()
{
	window_s w;

	w.width  = 67 + 2 + 2; /* 66 for all the info plus borders plus space */
	w.height = 10 + 2 + 1; /* 10 scores plus borders plus title line*/
	w.x      = engine.layout.main.width/2 - w.width/2 /* center */;
	w.y      = engine.layout.main.height/2 - w.height/2;
	w.win    = derwin(engine.layout.main.win, w.height, w.width, w.y, w.x);

	if (global.screen_fancy_borders)
		window_fancy_borders(w.win);
	else
		window_normal_borders(w.win);

	engine.layout.hscores_container = w;

	w.width  = engine.layout.hscores_container.width - 2;
	w.height = engine.layout.hscores_container.height - 2;
	w.x      = 1;
	w.y      = 1;
	w.win    = derwin(engine.layout.hscores_container.win, w.height, w.width, w.y, w.x);
	engine.layout.hscores = w;
}

/** Pops a window showing the top highscores
 *
 *  WTF LOCAL FUNCTION
 */
void engine_draw_hscores()
{
	window_s* w = NULL;

	w = &(engine.layout.hscores_container);
	if (global.screen_fancy_borders)
		window_fancy_borders(w->win);
	else
		window_normal_borders(w->win);

	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w->win, 0, 1, "High Scores");
	wnoutrefresh(w->win);

	w = &(engine.layout.hscores);
	werase(w->win);

	/* This is all well-alligned, think twice before changing a single char */
	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w->win, 0, 1, "     Score      Lines Level    Timer       Name       Date     Time");
	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, true));

	int i;
	for (i = 0; i < MAX_HSCORES; i++)
	{
		if (hscores[i].points == 0) /* empty score record */
			mvwaddstr(w->win, 1 + i, 1, "---------- ----------    -- -------- ---------- ---------- --------");
		else
			mvwprintw(w->win, 1 + i, 1, "%10d %10d    %2d %8s %10s %10s %8s",
			          hscores[i].points, hscores[i].lines, hscores[i].level, hscores[i].timer, hscores[i].name, hscores[i].date, hscores[i].time);
	}

	wnoutrefresh(w->win);
}

void engine_delete_hscores_window()
{
	delwin(engine.layout.hscores.win);

	werase(engine.layout.main.win);
	wnoutrefresh(engine.layout.main.win);
}

void engine_delete_help()
{
	delwin(engine.layout.help.win);

	werase(engine.layout.main.win);
	wnoutrefresh(engine.layout.main.win);
}

void engine_create_input()
{
	window_s w;

	w.width  = 50;
	w.height = 6;
	w.x      = engine.layout.main.width/2 - w.width/2 /* center */;
	w.y      = engine.layout.main.height/2 - w.height/2;
	w.win    = derwin(engine.layout.main.win, w.height, w.width, w.y, w.x);

	if (global.screen_fancy_borders)
		window_fancy_borders(w.win);
	else
		window_normal_borders(w.win);

	engine.layout.input_container = w;

	w.width  = engine.layout.input_container.width - 2;
	w.height = engine.layout.input_container.height - 2;
	w.x      = 1;
	w.y      = 1;
	w.win    = derwin(engine.layout.input_container.win, w.height, w.width, w.y, w.x);
	engine.layout.input = w;

	curs_set(1);
}

void engine_draw_input()
{
	window_s* w = NULL;

	w = &(engine.layout.input_container);
	if (global.screen_fancy_borders)
		window_fancy_borders(w->win);
	else
		window_normal_borders(w->win);

	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
//  mvwaddstr(w->win, 0, 1, "Enter your name");
	wrefresh(w->win);

	w = &(engine.layout.input);
	werase(w->win);

	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, false));
	mvwaddstr(w->win, 0, 1, "Contrats, you made into the high score list!");
	mvwaddstr(w->win, 1, 1, "Enter your name:");
//  mvwprintf(w->win, 1, 1, "(position: %d", 192998);

	char name[11];
	memset(name, '\0', 11);

	if (getenv("USER") == NULL)
		strncpy(name, "player", 10);
	else
		strncpy(name, getenv("USER"), 10);

	wattrset(w->win, color_pair(COLOR_BLUE, COLOR_DEFAULT, true));
	mvwprintw(w->win, 2, 18, "(default: %s)", name);

	/* ncurses' refresh all windows */
	wrefresh(w->win);
}

void engine_get_hscore_name(char* name, int size)
{
	window_s* w   = &(engine.layout.input);
	WINDOW*   sub = derwin(w->win, 1, 11, 1, 18);
	werase(sub);
	wattrset(sub, color_pair(COLOR_BLACK, COLOR_BLUE, false));
	mvwhline(sub, 0, 0, ' ', 11);

	char buffer[256];
	memset(buffer, '\0', 256);

	wattrset(w->win, color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
	nodelay(stdscr, FALSE);
	echo();

	mvwscanw(sub, 0, 0, "%[^\n]s", buffer);
	if (strlen(buffer) != 0)
		strncpy(name, buffer, size);

	noecho();
	nodelay(stdscr, TRUE);
}

void engine_delete_input()
{
	delwin(engine.layout.input.win);
	curs_set(0);
	werase(engine.layout.main.win);
	wnoutrefresh(engine.layout.main.win);
}


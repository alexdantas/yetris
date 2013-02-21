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


/* Functions specific to this module
 * (engine-specific functions)
 */
void register_signal_handler();
void window_fancy_borders(WINDOW* win);
void window_normal_borders(WINDOW* win);


/** Start things related to the game screen and layout */
int engine_screen_init(int width, int height)
{
	engine.screen.width  = width;
	engine.screen.height = height;

	/* Starting ncurses! */
	initscr();

	if ((has_colors() == TRUE) && (global.screen_use_colors))
	{
		start_color();

		/* This is a big hack to initialize all possible colors
		 * in ncurses. The thing is, all colors are between
		 * COLOR_BLACK and COLOR_WHITE.
		 * Since I've set a large number of enums covering
		 * all possibilities, I can do it all in a for loop.
		 * Check 'man init_pair' for more details.
		 *
		 * This was taken straight from <curses.h>:
		 *
		 * #define COLOR_BLACK	 0
		 * #define COLOR_RED	 1
		 * #define COLOR_GREEN	 2
		 * #define COLOR_YELLOW	 3
		 * #define COLOR_BLUE	 4
		 * #define COLOR_MAGENTA 5
		 * #define COLOR_CYAN	 6
		 * #define COLOR_WHITE	 7
		 */
		int i, j, k = 1;
		for (i = COLOR_BLACK; i <= COLOR_WHITE; i++)
		{
			for (j = COLOR_BLACK; j <= COLOR_WHITE; j++)
			{
				init_pair(k, i, j);
				k++;
			}
		}
	}

	/* Gets the current width and height */
	int current_height, current_width;
	getmaxyx(stdscr, current_height, current_width);

	if ((current_width  < engine.screen.width) ||
	    (current_height < engine.screen.height))
	{
		endwin();

/* for now i must keep this - windows doesnt handle stderr */
#if OS_IS_WINDOWS
		printf(
#else
		fprintf(stderr,
#endif
		                "Error! Your console screen is smaller than %dx%d\n"
		                "Please resize your window and try again\n",
		                engine.screen.width, engine.screen.height);

		exit(EXIT_FAILURE);
	}
	engine.screen.width  = current_width;
	engine.screen.height = current_height;

	cbreak();    /* Character input doesnt require the <enter> key anymore */
	curs_set(0); /* Makes the blinking cursor invisible */
	noecho();    /* Wont print the keys received through input */
	nodelay(stdscr, TRUE); /* Wont wait for input */
	keypad(stdscr, TRUE);  /* Support for extra keys (life F1, F2, ... ) */
	refresh();   /* Refresh the screen (prints whats in the screen buffer) */
	return 1;
}

/** Starts all the subscreens of the game */
int engine_windows_init()
{
	window_s  w;
	screen_s* s = &(engine.screen);

	int main_x = 0;
	int main_y = 0;
	if (global.screen_center_horizontally)
		main_x = engine.screen.width/2 - 80/2;

	if (global.screen_center_vertically)
		main_y = engine.screen.height/2 - 24/2;

	/* main */
	w.width  = 80;
	w.height = 24;
	w.x      = main_x;
	w.y      = main_y;
	w.win    = newwin(w.height, w.width, w.y, w.x);
	if (global.screen_show_outer_border)
	{
		if (global.screen_fancy_borders)
			window_fancy_borders(w.win);
		else
			window_normal_borders(w.win);
	}

	wrefresh(w.win);
	s->main = w;

	/* leftmost */
	w.width  = 6 * 2 + 2;
	w.height = s->main.height - 2; /* borders */
	w.x      = 2;
	w.y      = 1;
	w.win    = derwin(s->main.win, w.height, w.width, w.y, w.x);

	if (global.screen_fancy_borders)
	{
		window_fancy_borders(w.win);

		/* If the player has no hold, doesnt make sense printing these parts */
		if (global.game_can_hold)
		{
			/* making the top line between hold and score windows */
			mvwaddch(w.win, 5, 0, ACS_LLCORNER|COLOR_PAIR(WHITE_BLACK));

/* dang, PDCurses (windows) doesnt have mvwhline */
#if !OS_IS_WINDOWS
			mvwhline(w.win, 5, 1, ACS_HLINE|COLOR_PAIR(BLACK_BLACK)|A_BOLD, w.width - 2);
#endif
			mvwaddch(w.win, 5, w.width - 1, ACS_LRCORNER|COLOR_PAIR(BLACK_BLACK)|A_BOLD);
			/* making the bottom line between hold and score windows */
			mvwaddch(w.win, 6, 0, ACS_ULCORNER|COLOR_PAIR(WHITE_BLACK)|A_BOLD);

/* dang, PDCurses (windows) doesnt have mvwhline */
#if !OS_IS_WINDOWS
			mvwhline(w.win, 6, 1, ACS_HLINE|COLOR_PAIR(WHITE_BLACK), w.width - 2);
#endif
			mvwaddch(w.win, 6, w.width - 1, ACS_URCORNER|COLOR_PAIR(WHITE_BLACK));
		}

	}
	else
	{
		window_normal_borders(w.win);
		wattron(w.win, engine_get_color(COLOR_BLACK, COLOR_BLACK, true));

/* dang, PDCurses (windows) doesnt have mvwhline */
#if !OS_IS_WINDOWS
		mvwhline(w.win, 5, 1, '-', w.width - 2);
#endif
	}

	wrefresh(w.win);
	s->leftmost = w;

	/* middle-left */
	w.width  = 10 * 2 + 2;
	w.height = s->main.height - 2; /* borders */
	w.x      = s->leftmost.x + s->leftmost.width + 1;
	w.y      = 1;
	w.win    = derwin(s->main.win, w.height, w.width, w.y, w.x);
	if (global.screen_fancy_borders)
		window_fancy_borders(w.win);
	else
		window_normal_borders(w.win);
	wrefresh(w.win);
	s->middle_left = w;

	/* middle-right */
	w.width  = 4 * 2 + 2;
	w.height = s->main.height - 2; /* borders */
	w.x      = s->middle_left.x + s->middle_left.width + 1;
	w.y      = 1;
	w.win    = derwin(s->main.win, w.height, w.width, w.y, w.x);
	if (global.screen_fancy_borders)
	{
		window_fancy_borders(w.win);
		/* making the top line between 1st next and the rest */
		mvwaddch(w.win, 3, 0, ACS_LLCORNER|COLOR_PAIR(WHITE_BLACK));

/* dang, PDCurses (windows) doesnt have mvwhline */
#if !OS_IS_WINDOWS
		mvwhline(w.win, 3, 1, ACS_HLINE|COLOR_PAIR(BLACK_BLACK)|A_BOLD, w.width - 2);
#endif
		mvwaddch(w.win, 3, w.width - 1, ACS_LRCORNER|COLOR_PAIR(BLACK_BLACK)|A_BOLD);
		/* making the bottom line between 1st next and the rest */
		mvwaddch(w.win, 4, 0, ACS_ULCORNER|COLOR_PAIR(WHITE_BLACK)|A_BOLD);

/* dang, PDCurses (windows) doesnt have mvwhline */
#if !OS_IS_WINDOWS
		mvwhline(w.win, 4, 1, ACS_HLINE|COLOR_PAIR(WHITE_BLACK), w.width - 2);
#endif
		mvwaddch(w.win, 4, w.width - 1, ACS_URCORNER|COLOR_PAIR(WHITE_BLACK));

	}
	else
	{
		window_normal_borders(w.win);
		wattron(w.win, engine_get_color(COLOR_BLACK, COLOR_BLACK, true));

/* dang, PDCurses (windows) doesnt have mvwhline */
#if !OS_IS_WINDOWS
		mvwhline(w.win, 3, 1, '-', w.width - 2);
#endif

	}
	wrefresh(w.win);
	s->middle_right = w;

	/* right-most */
	w.width  = s->main.width - (s->middle_right.x + s->middle_right.width) - 3;
	w.height = s->main.height - 2; /* borders */
	w.x      = s->middle_right.x + s->middle_right.width + 1;
	w.y      = 1;
	w.win    = derwin(s->main.win, w.height, w.width, w.y, w.x);
	if (global.screen_fancy_borders)
		window_fancy_borders(w.win);
	else
		window_normal_borders(w.win);
	wrefresh(w.win);
	s->rightmost = w;

	/* next pieces */
	w.width  = s->middle_right.width  - 2;
	w.height = s->middle_right.height - 2;
	w.x      = 1;
	w.y      = 1;
	w.win    = derwin(s->middle_right.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->next_container = w;

	/* first next piece */
	w.width  = s->next_container.width;
	w.height = 2;
	w.x      = 0;
	w.y      = 0;
	w.win    = derwin(s->next_container.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->next[0] = w;

	/* the rest */
	int i; int y_offset = 2;
	for (i = 1; i <= global.game_next_no; i++)
	{
		/* making all the next pieces 1 line lower */
		if (i != 1)
			y_offset = 0;

		w.width  = s->next_container.width;
		w.height = 2;
		w.x      = 0;
		w.y      = s->next[i - 1].y + s->next[i - 1].height + 1 + y_offset;
		w.win    = derwin(s->next_container.win, w.height, w.width, w.y, w.x);
		wrefresh(w.win);
		s->next[i] = w;
	}

	/* game board */
	w.width  = s->middle_left.width  - 2;
	w.height = s->middle_left.height - 2;
	w.x      = 1;
	w.y      = 1;
	w.win    = derwin(s->middle_left.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->board = w;

	/* info */
	w.width  = s->rightmost.width  - 4;
	w.height = s->rightmost.height - 2;
	w.x      = 2;
	w.y      = 1;
	w.win    = derwin(s->rightmost.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->info = w;

	/* leftmost container */
	w.width  = s->leftmost.width  - 2;
	w.height = s->leftmost.height - 2;
	w.x      = 1;
	w.y      = 1;
	w.win    = derwin(s->leftmost.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->leftmost_container = w;

	/* hold */
	w.width  = s->leftmost_container.width;
	w.height = 4;
	w.x      = 0;
	w.y      = 0;
	w.win    = derwin(s->leftmost_container.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->hold = w;

	/* score screen */
	w.width  = s->leftmost_container.width;
	w.height = s->leftmost_container.height - (s->hold.height) - 2;
	w.x      = 0;
	w.y      = s->hold.y + s->hold.height + 2;
	w.win    = derwin(s->leftmost_container.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->score = w;

	w = s->info;
	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, true));
	mvwaddstr(w.win, w.height - 1, 16 , "Loading");
	wrefresh(w.win);
	return 1;
}

/** Initializes all ncurses' related stuff (windows, colors...).
 *  There's no need to call 'engine_exit' */
bool engine_init()
{
	/* signals during initialization */
	block_signals();

	engine_screen_init(80, 24);
	engine_windows_init();
	engine_keymap(NULL);

	restore_signals();
	register_signal_handler();
	return true;
}

/** Whenever we get a signal from the player or the system,
 *  we're calling engine_safe_exit to safely exit ncurses
 */
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

/** This function blocks the interrupt signal (Ctrl+C) during
 *  the game's initialization.
 *  That's because ncurses leaves the terminal in a broken state.
 */
bool block_signals()
{
	struct sigaction sigint_handler;
	sigint_handler.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sigint_handler, NULL);
	return true;
}

/** Now the player's allowed to interrupt the program it he wishes so. */
bool restore_signals()
{
	struct sigaction sigint_handler;
	sigint_handler.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sigint_handler, NULL);
	return true;
}

/** This stops ncurses on a sweet, gentle way. */
void engine_exit()
{
	erase();
	refresh();
	endwin();
}

/** Function called when receiving an interrupt signal.
 *  It restores the terminal to it's initial state and
 *  frees whatever memory might be allocated from the game.
 */
void engine_safe_exit(int sig)
{
	engine_exit();

	/* since the game doesn't deal with malloc (yet) we're
	 * pretty much safe quitting like this.
	 * I've already set that when we call exit() we quit
	 * from the engine, anyway.
	 */
#if OS_IS_WINDOWS
		printf(
#else
		fprintf(stderr,
#endif
		        "Interrupted (signal %d).\n"
		        "Bad game, no donut for you!\n", sig);

	/* won't call functions registered with atexit() */
	_exit(EXIT_FAILURE);
}

/** This defines the keymap according to the string #keymap.
 *   Each char represents the key to be pressed for the following
 *   commands (on that order):
 *     down, right, left, rotate, rotate backwards, drop, pause, quit, hold
 *   For example:
 *     "sdawe pqu"
 */
int engine_keymap(char keymap[])
{
	/* TODO: check for repeated letters */

	/* ncurses' constants */
	engine.input.none = ERR;

	if ((!keymap) || (strnlen(keymap, NUMBER_OF_KEYS + 1) != NUMBER_OF_KEYS))
	{
		/* Invalid string, setting default keymap */
		engine.input.down   = KEY_DOWN;
		engine.input.right  = KEY_RIGHT;
		engine.input.left   = KEY_LEFT;
		engine.input.rotate = 'x';
		engine.input.rotate_backw = 'z';
		engine.input.drop    = ' ';
		engine.input.hold    = 'c';
		engine.input.pause   = 'p';
		engine.input.quit    = 'q';
		engine.input.restart = 'r';
		return -1;
	}

	engine.input.down   = keymap[0];
	engine.input.right  = keymap[1];
	engine.input.left   = keymap[2];
	engine.input.rotate = keymap[3];
	engine.input.rotate_backw = keymap[4];
	engine.input.drop   = keymap[5];
	engine.input.pause  = keymap[6];
	engine.input.quit   = keymap[7];
	engine.input.hold   = keymap[8];
	engine.input.restart = keymap[8];
	return 0;
}

/** Get input, waiting at most #delay_ms miliseconds.
 *  @return An input enum - it could be ERROR, you know */
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

/** Draws a single block.
 *  It prints on the screen the two chars that represents
 *  the block. They're stored on #b under 'theme'.
 */
void engine_draw_block(block_s* b, WINDOW* w)
{
	if (global.screen_use_colors)
		wattron(w, b->color);
	else
		wattron(w, engine_get_color(COLOR_BLACK, COLOR_WHITE, false));

	mvwaddch(w, b->y, (b->x * 2),     b->theme[0]);
	mvwaddch(w, b->y, (b->x * 2) + 1, b->theme[1]);
}

/** Draws a whole piece, calling #engine_draw_block */
void engine_draw_piece(piece_s* p, WINDOW* w)
{
	if (!piece_is_valid(p))
		return;

	int k;
	for (k = 0; k < 4; k++)
		engine_draw_block(&(p->block[k]), w);
}

/** Draws the board #b, calling #engine_draw_block */
void engine_draw_board(board_s* b)
{
	WINDOW* w = engine.screen.board.win;

	int i, j;
	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			if (b->block[i][j].type != EMPTY)
				engine_draw_block(&(b->block[i][j]), w);
}

/** Prints 'pause' on the board */
void engine_draw_pause()
{
	window_s* w = &(engine.screen.board);

	wattron(w->win, engine_get_color(COLOR_BLUE, COLOR_BLACK, false));
	mvwaddstr(w->win, w->height/2 - 1, w->width/2 - 4, "[paused]");
	wrefresh(w->win);
}

void engine_draw_next_pieces(game_s* g)
{
	WINDOW* w = NULL;
	int i, k;
	for (i = 0; i < global.game_next_no; i++)
	{
		piece_s p = g->piece_next[i];
		w = engine.screen.next[i].win;

		werase(w);

		/* This is a little hack to pretty-print pieces
		 * TODO somehow manage to fix this */
		for (k = 0; k < 4; k++)
		{
			/* shifting them to the left */
			p.block[k].x -= p.x + 1;
			p.block[k].y -= p.y;

			p.block[k].y--;

			if (p.type == PIECE_O)
				p.block[k].y -= 1;
		}
		engine_draw_piece(&p, w);
		wrefresh(w);
	}

	w = engine.screen.middle_right.win;

	if (global.screen_fancy_borders)
	{
		mvwaddch(w, 3, 0, ACS_LLCORNER|COLOR_PAIR(WHITE_BLACK));

/* dang, PDCurses (windows) doesnt have mvwhline */
#if !OS_IS_WINDOWS
		mvwhline(w, 3, 1, ACS_HLINE|COLOR_PAIR(BLACK_BLACK)|A_BOLD, 8);
#endif
		mvwaddch(w, 3, 9, ACS_LRCORNER|COLOR_PAIR(BLACK_BLACK)|A_BOLD);
	}
	else
	{
		wattron(w, engine_get_color(COLOR_BLACK, COLOR_BLACK, true));

/* dang, PDCurses (windows) doesnt have mvwhline */
#if !OS_IS_WINDOWS
		mvwhline(w, 3, 1, '-', 8);
#endif
	}

	wattron(w, engine_get_color(COLOR_BLUE, COLOR_BLACK, false));
	mvwaddstr(w, 0, 1, "Next");
	wrefresh(w);
}

void engine_draw_hold(game_s* g)
{
	piece_s  p = g->piece_hold;

	window_s w = engine.screen.leftmost;

	wattron(w.win, engine_get_color(COLOR_BLUE, COLOR_BLACK, false));
	mvwaddstr(w.win, 0, 1, "Hold");
	wrefresh(w.win);

	w = engine.screen.hold;
	werase(w.win);
	engine_draw_piece(&p, w.win);
	wrefresh(w.win);
}


void engine_draw_score(game_s* g)
{
	window_s w = engine.screen.score;

	werase(w.win);

	/* If user has combo, let's show it to him */
	if ((g->is_combo) && (g->combo_count > 0))
	{
		if (g->combo_count > 3)
			wattron(w.win, engine_get_color(COLOR_RED, COLOR_BLACK, false));
		else
			wattron(w.win, engine_get_color(COLOR_YELLOW, COLOR_BLACK, false));

		mvwaddstr(w.win, 0, 2, "Combo!");
		mvwprintw(w.win, 0, 8, "x%d", g->combo_count);
	}

	/* If user has back-to-back lines, let's show it to him */
	if (g->back_to_back_count > 0)
	{
		if (g->back_to_back_lines < 4)
			wattron(w.win, engine_get_color(COLOR_RED,    COLOR_BLACK, true));
		else
			wattron(w.win, engine_get_color(COLOR_YELLOW, COLOR_BLACK, true));

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

	wattron(w.win, engine_get_color(COLOR_BLUE, COLOR_BLACK, false));
	mvwaddstr(w.win, 3,  1, "High Score");
	mvwaddstr(w.win, 6,  1, "Score");
	mvwaddstr(w.win, 9,  1, "Lines");
	mvwaddstr(w.win, 12, 1, "Level");

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, 4,  1, "%10d", g->hscore);
	mvwprintw(w.win, 7,  1, "%10d", g->score);
	mvwprintw(w.win, 10, 1, "%10d", g->lines);
	mvwprintw(w.win, 13, 9, "%02d", g->level);

	wrefresh(w.win);
}

/** This is a very ugly function that draws blocks on the
 *  info window for statistical purposes.
 *  It uses several position hacks that i need to fix later.
 *  The problem is that pieces always start relative to the
 *  middle of the board, independent of the screen.
 *  So im repositioning them according to it, on the info screen.
 */
void engine_draw_statistics(game_s* g)
{
	window_s w = engine.screen.info;
	int k;
	int x_offset = 3;

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, 3, 1, "%10d x", g->I_count);
	piece_s  p = new_piece(PIECE_I);
	p.x = x_offset;
	p.y = 4;
	for (k = 0; k < 4; k++)
	{
		p.block[k].x += p.x;
		p.block[k].y += p.y;
	}
	engine_draw_piece(&p, w.win);

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, 5, 1, "%10d x", g->T_count);
	p = new_piece(PIECE_T);
	p.x = x_offset;
	p.y = 6;
	for (k = 0; k < 4; k++)
	{
		p.block[k].x += p.x;
		p.block[k].y += p.y;
	}
	engine_draw_piece(&p, w.win);

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, 7, 1, "%10d x", g->L_count);
	p = new_piece(PIECE_L);
	p.x = x_offset;
	p.y = 8;
	for (k = 0; k < 4; k++)
	{
		p.block[k].x += p.x;
		p.block[k].y += p.y;
	}
	engine_draw_piece(&p, w.win);

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, 9, 1, "%10d x", g->J_count);
	p = new_piece(PIECE_J);
	p.x = x_offset;
	p.y = 10;
	for (k = 0; k < 4; k++)
	{
		p.block[k].x += p.x;
		p.block[k].y += p.y;
	}
	engine_draw_piece(&p, w.win);

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, 11, 1, "%10d x", g->S_count);
	p = new_piece(PIECE_S);
	p.x = x_offset;
	p.y = 12;
	for (k = 0; k < 4; k++)
	{
		p.block[k].x += p.x;
		p.block[k].y += p.y;
	}
	engine_draw_piece(&p, w.win);

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, 13, 1, "%10d x", g->Z_count);
	p = new_piece(PIECE_Z);
	p.x = x_offset;
	p.y = 14;
	for (k = 0; k < 4; k++)
	{
		p.block[k].x += p.x;
		p.block[k].y += p.y;
	}
	engine_draw_piece(&p, w.win);

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, 15, 1, "%10d x", g->O_count);
	p = new_piece(PIECE_O);
	p.x = x_offset - 1;
	p.y = 16;
	for (k = 0; k < 4; k++)
	{
		p.block[k].x += p.x;
		p.block[k].y += p.y;
	}
	engine_draw_piece(&p, w.win);

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, 16, 1, "%10d   Total", g->piece_count);
}

/** Draws everything that's on the info window (the rightmost one) */
void engine_draw_info(game_s* g)
{
	window_s w = engine.screen.info;

	werase(w.win);

	if (global.game_has_statistics)
		engine_draw_statistics(g);

	wattron(w.win, engine_get_color(COLOR_BLUE, COLOR_BLACK, false));
	mvwaddstr(w.win, 0, 0, "yetris v"VERSION);
	mvwaddstr(w.win, 1, 1, "('yetris -h' for info)");
	mvwaddstr(w.win, w.height - 1, 0, "Timer:");

	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	if (g->gameplay_h) /* Wow.. will someone really play this game for hours? */
		mvwprintw(w.win, w.height - 1, 7, "%02d:%02d:%02d", g->gameplay_h, g->gameplay_m % 60, g->gameplay_s % 60);
	else
		mvwprintw(w.win, w.height - 1, 7, "%02d:%02d", g->gameplay_m % 60, g->gameplay_s % 60);

	wattron(w.win, engine_get_color(COLOR_BLUE, COLOR_BLACK, false));
	mvwaddstr(w.win, w.height - 2, 0, "Speed:");
	wattron(w.win, engine_get_color(COLOR_WHITE, COLOR_BLACK, false));
	mvwprintw(w.win, w.height - 2, 7, "%dms", g->speed);

	/** DEBUG INFO */
	/* window_color(w.win, BLUE_BLACK, false); */
	/* mvwprintw(w.win, 7, 0, "Back-To-Back: %d", g->is_back_to_back); */
	/* window_color(w.win, WHITE_BLACK, false); */
	/* mvwprintw(w.win, 8, 0, "Count:        %d", g->back_to_back_count); */
	/* mvwprintw(w.win, 9, 0, "Lines:        %d", g->back_to_back_lines); */


	/* This is a little hack to display the time onscreen.
	 * It's ugly as hell, but I had to make it
	 * Format: Wed Jun 30 21:49:08 1993\n */
	time_t cur_time;
	time(&cur_time);
	wattron(w.win, engine_get_color(COLOR_BLACK, COLOR_BLACK, true));
	mvwprintw(w.win, w.height - 1, 15, "%.8s", (ctime(&cur_time) + 11));

	wrefresh(w.win);
}

/** Returns the color pair associated with #color.
 *  If #is_bold is true, will make the color brighter.
 */
//int engine_get_color(color_e color, bool is_bold)
int engine_get_color(short foreground, short background, bool is_bold)
{
	color_e c = (foreground * 8) + (background + 1);
	if (is_bold)
		return (COLOR_PAIR(c) | A_BOLD);
	else
		return COLOR_PAIR(c);

	/* int col = COLOR_PAIR(color); */
	/* if (is_bold) */
	/* 	col = col | A_BOLD; */
	/* 	/\* TRY TO MAKE THIS WORK *\/ */

	/* return col; */
}

/** Calls all drawing routines in order */
void engine_draw(game_s* g)
{
	WINDOW* w = engine.screen.board.win;
	werase(w);

	switch (g->state)
	{
	case PLAYING:
		engine_draw_board(&(g->board));
		engine_draw_piece(&(g->piece_ghost), w);
		engine_draw_piece(&(g->piece_current), w);
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

	default: /* Umm... Nothing, I guess...? */ break;
	}

	wrefresh(w);
}

/** Draws a little animation on the board, painting all pieces white. */
void engine_draw_gameover(game_s* g)
{
	board_s* b = &(g->board);
	WINDOW*  w = engine.screen.board.win;

	int i, j;
	for (j = 0; j < BOARD_HEIGHT; j++)
	{
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			if (b->block[i][j].type != EMPTY)
			{
				b->block[i][j].color = engine_get_color(COLOR_BLACK, COLOR_WHITE, false);
				engine_draw_block(&(b->block[i][j]), w);
			}
		}
		/* Stop a little (50ms) before painting next line */
		usleep(50000);
		wrefresh(w);
	}
	/* Now wait a second, to let the feeling of defeat sink in */
	wrefresh(w);
	usleep(1000000);
}

/** Gets a single keypress and them return to normal game. */
void engine_wait_for_keypress()
{

/* windows doesnt recognize stdin */
#if !OS_IS_WINDOWS
	fflush(stdin); /* discard any characters pressed until now */
#endif

	nodelay(stdscr, FALSE);
	getch();
	nodelay(stdscr, TRUE);
}

/* /\** Pops a window explaining some stuff *\/ */
/* void engine_draw_help() */
/* { */
/* 	screen_s* s = &(engine.screen); */
/* 	window_s* w; /\* help screen (only shows when requested) *\/ */

/* 	w->width  = 40; */
/* 	w->height = 10; */
/* 	w->x      = 0;//s->main.width/2  - 40/2; */
/* 	w->y      = 0;//s->main.height/2 - 10/2; */
/* 	w->win    = derwin(s->main.win, w->height, w->width, w->y, w->x); */
/* 	wborder(w->win, '|', '|', '-', '-', '+', '+', '+', '+'); */

/* //	mvwaddstr(w->win, 0, 4, "Help"); */

/* 	wrefresh(w->win); */
/* 	delwin(w->win); */
/* 	wrefresh(s->main.win); */
/* } */

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * ENGINE-SPECIFIC FUNCTIONS
 * (invisible to the external world)
 * * * * * * * * * * * * * * * * * * * * * * * * * */

/* /\** Turns on color #color on window #win. *\/ */
/* void window_color(WINDOW* win, int color, bool is_bold) */
/* { */
/* 	if (global.screen_use_colors) */
/* 		wattron(win, engine_get_color(color, is_bold)); */
/* } */

/** Draws fancy borders on window #win */
void window_fancy_borders(WINDOW* win)
{
	wborder(win, ACS_VLINE|COLOR_PAIR(WHITE_BLACK),
			ACS_VLINE|COLOR_PAIR(BLACK_BLACK)|A_BOLD,
			ACS_HLINE|COLOR_PAIR(WHITE_BLACK),
			ACS_HLINE|COLOR_PAIR(BLACK_BLACK)|A_BOLD,
			ACS_ULCORNER|COLOR_PAIR(WHITE_BLACK)|A_BOLD,
			ACS_URCORNER|COLOR_PAIR(WHITE_BLACK),
			ACS_LLCORNER|COLOR_PAIR(WHITE_BLACK),
			ACS_LRCORNER|COLOR_PAIR(BLACK_BLACK)|A_BOLD);
}

/** Draws normal borders on window #win */
void window_normal_borders(WINDOW* win)
{
	wattron(win, engine_get_color(COLOR_BLACK, COLOR_BLACK, true));
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
}


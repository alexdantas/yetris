

#include <stdlib.h>
#include <ncurses.h>

#include <sys/time.h>  /* select() */
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "engine.h"
#include "game.h"
#include "globals.h"

/** Start things related to the game screen and layout */
int engine_screen_init(int width, int height)
{
	engine.screen.width  = width;
	engine.screen.height = height;

	/* Starting ncurses! */
	initscr();

	if (has_colors() == TRUE)
	{
		start_color();
		/* Colors (Enum Name,     Foreground,    Background) */
		init_pair(GREEN_BLACK,   COLOR_GREEN,   COLOR_BLACK);
		init_pair(CYAN_BLACK,    COLOR_CYAN,    COLOR_BLACK);
		init_pair(WHITE_BLACK,   COLOR_WHITE,   COLOR_BLACK);
		init_pair(RED_BLACK,     COLOR_RED,     COLOR_BLACK);
		init_pair(BLUE_BLACK,    COLOR_BLUE,    COLOR_BLACK);
		init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(YELLOW_BLACK,  COLOR_YELLOW,  COLOR_BLACK);
		init_pair(WHITE_BLACK,   COLOR_WHITE,   COLOR_BLACK);
		init_pair(BLACK_GREEN,   COLOR_BLACK,   COLOR_GREEN);
		init_pair(BLACK_CYAN,    COLOR_BLACK,   COLOR_CYAN);
		init_pair(BLACK_WHITE,   COLOR_BLACK,   COLOR_WHITE);
		init_pair(BLACK_RED,     COLOR_BLACK,   COLOR_RED);
		init_pair(BLACK_BLUE,    COLOR_BLACK,   COLOR_BLUE);
		init_pair(BLACK_MAGENTA, COLOR_BLACK,   COLOR_MAGENTA);
		init_pair(BLACK_YELLOW,  COLOR_BLACK,   COLOR_YELLOW);
		init_pair(BLACK_WHITE,   COLOR_BLACK,   COLOR_WHITE);
	}

	/* Gets the current width and height */
	int current_height, current_width;
	getmaxyx(stdscr, current_height, current_width);

	if ((current_width  < engine.screen.width) ||
	    (current_height < engine.screen.height))
	{
		engine_exit();
		fprintf(stderr, "Error! Your console screen is smaller than %dx%d\n"
		                "Please resize your window and try again\n",
		                engine.screen.width, engine.screen.height);

		exit(EXIT_FAILURE);
	}
	engine.screen.width  = current_width;
	engine.screen.height = current_height;

	raw();       /* Character input doesnt require the <enter> key anymore */
	curs_set(0); /* Makes the blinking cursor invisible */
	noecho();    /* Wont print the keys received through input */
	nodelay(stdscr, TRUE); /* Wont wait for input */
	keypad(stdscr, TRUE);  /* Support for extra keys (life F1, F2, ... ) */
	refresh();   /* Refresh the screen (prints whats in the screen buffer) */
}

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
	box(w.win, 0, 0);
	wrefresh(w.win);
	s->main = w;

	/* leftmost */
	w.width  = 6 * 2 + 2;
	w.height = s->main.height - 2; /* borders */
	w.x      = 2;
	w.y      = 1;
	w.win    = derwin(s->main.win, w.height, w.width, w.y, w.x);
	wborder(w.win, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(w.win);
	s->leftmost = w;

	/* middle-left */
	w.width  = 10 * 2 + 2;
	w.height = s->main.height - 2; /* borders */
	w.x      = s->leftmost.x + s->leftmost.width + 1;
	w.y      = 1;
	w.win    = derwin(s->main.win, w.height, w.width, w.y, w.x);
	wborder(w.win, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(w.win);
	s->middle_left = w;

	/* middle-right */
	w.width  = 4 * 2 + 2;
	w.height = s->main.height - 2; /* borders */
	w.x      = s->middle_left.x + s->middle_left.width + 1;
	w.y      = 1;
	w.win    = derwin(s->main.win, w.height, w.width, w.y, w.x);
	wborder(w.win, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(w.win);
	s->middle_right = w;

	/* right-most */
	w.width  = s->main.width - (s->middle_right.x + s->middle_right.width) - 3;
	w.height = s->main.height - 2; /* borders */
	w.x      = s->middle_right.x + s->middle_right.width + 1;
	w.y      = 1;
	w.win    = derwin(s->main.win, w.height, w.width, w.y, w.x);
	wborder(w.win, '|', '|', '-', '-', '+', '+', '+', '+');
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

	w.width  = s->next_container.width;
	w.height = 5;
	w.x      = 0;
	w.y      = 0;
	w.win    = derwin(s->next_container.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->next[0] = w;

	w.width  = s->next_container.width;
	w.height = 5;
	w.x      = 0;
	w.y      = s->next[0].y + s->next[0].height - 1;
	w.win    = derwin(s->next_container.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->next[1] = w;

	w.width  = s->next_container.width;
	w.height = 5;
	w.x      = 0;
	w.y      = s->next[1].y + s->next[1].height - 1;
	w.win    = derwin(s->next_container.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->next[2] = w;

	w.width  = s->next_container.width;
	w.height = 5;
	w.x      = 0;
	w.y      = s->next[2].y + s->next[2].height - 1;
	w.win    = derwin(s->next_container.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->next[3] = w;

	w.width  = s->next_container.width;
	w.height = 4;
	w.x      = 0;
	w.y      = s->next[3].y + s->next[3].height - 1;
	w.win    = derwin(s->next_container.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->next[4] = w;

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
	w.height = 5;
	w.x      = 0;
	w.y      = 0;
	w.win    = derwin(s->leftmost_container.win, w.height, w.width, w.y, w.x);
	mvwhline(w.win, w.height - 1, 0, '-', w.width);
	wrefresh(w.win);
	s->hold = w;

	/* score screen */
	w.width  = s->leftmost_container.width;
	w.height = s->leftmost_container.height - (s->hold.height + 1);
	w.x      = 0;
	w.y      = s->hold.y + s->hold.height;
	w.win    = derwin(s->leftmost_container.win, w.height, w.width, w.y, w.x);
	wrefresh(w.win);
	s->score = w;

	w = s->info;
	mvwaddstr(w.win, w.height - 1, 16 , "Loading");
	wrefresh(w.win);
}

/** Initializes all ncurses' related stuff (windows, colors...).
 *  There's no need to call 'engine_exit' */
int engine_init()
{
	/* Block signals while initializing ncurses, otherwise the
	 * console will be screwed up */
	block_signals();

	engine_screen_init(80, 24);
	engine_windows_init();
	engine_keymap(NULL);
	srand(time(NULL));

	restore_signals();
}

int block_signals()
{
//	sigprocmask(SIG_BLOCK);
}

int restore_signals()
{
//	sigprocmask(SIG_SETMASK);
}

void engine_exit()
{
	erase();
	refresh();
	endwin();
}

/** This defines the keymap according to the string #keymap.
 *  Each char represents the key to be pressed for the following
 *  commands (on that order):
 *    down, right, left, rotate, rotate backwards, drop, pause, quit
 *  For example:
 *    "sdawe pq"
 */
int engine_keymap(char keymap[])
{
	/* TODO: check for repeated letters */

	/* ncurses' constants */
	engine.input.none = ERR;

	if ((!keymap) || (strnlen(keymap, 9) != 8))
	{
		/* Invalid string, setting default keymap */
		engine.input.down   = KEY_DOWN;
		engine.input.right  = KEY_RIGHT;
		engine.input.left   = KEY_LEFT;
		engine.input.rotate = 'z';
		engine.input.rotate_backw = 'x';
		engine.input.drop   = ' ';
		engine.input.pause  = '\n';
		engine.input.quit   = 'q';
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

/* wont call refresh */
void engine_draw_block(block_s* b, WINDOW* w)
{
	wattrset(w, b->color);
	mvwaddstr(w, b->y, (b->x * 2), b->theme);
}

void engine_draw_piece(piece_s* p, WINDOW* w)
{
	if (!piece_is_valid(p))
		return;

	int k;
	for (k = 0; k < 4; k++)
		engine_draw_block(&(p->block[k]), w);
}

void engine_draw_board(board_s* b)
{
	WINDOW* w = engine.screen.board.win;

	int i, j;
	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			if (b->block[i][j].type != EMPTY)
				engine_draw_block(&(b->block[i][j]), w);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/* TODO draw next pieces 2-block width -- make them all layed down */
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



void engine_draw_next_pieces(game_s* g)
{
	WINDOW* w = NULL;
	int i, k;
	for (i = 0; i < NEXT_PIECES_NO; i++)
	{
		piece_s p = g->piece_next[i];
		w = engine.screen.next[i].win;

		werase(w);

		for (k = 0; k < 4; k++)
		{
			p.block[k].x -= p.x + 1; /* shifting them to the left */
			p.block[k].y -= p.y;

			if (p.type == PIECE_J) /* Pretty-printing */
				p.block[k].x += 1;
		}
		engine_draw_piece(&p, w);
		wrefresh(w);
	}

	w = engine.screen.next[0].win;
	wattron(w, COLOR_PAIR(WHITE_BLACK));
	mvwhline(w, 4, 0, '-', 12);
	wrefresh(w);

	w = engine.screen.next_container.win;
	wattron(w, COLOR_PAIR(WHITE_BLACK));
	mvwaddstr(w, 0, 0, "Next");
	wrefresh(w);
}

void engine_draw_hold(game_s* g)
{
	window_s w = engine.screen.hold;
	piece_s  p = g->piece_hold;

	werase(w.win);

	mvwaddstr(w.win, 0, 0, "Hold");
	engine_draw_piece(&p, w.win);
	wattrset(w.win, COLOR_PAIR(WHITE_BLACK));
	mvwhline(w.win, w.height - 1, 0, '-', w.width);

	wrefresh(w.win);
}

void engine_draw_score(game_s* g)
{
	window_s w = engine.screen.score;
	int offset = 6;

	mvwaddstr(w.win, offset + 0, 1, "Score");
	mvwprintw(w.win, offset + 1, 1, "%10d", g->score);

	mvwaddstr(w.win, offset + 3, 1, "Lines");
	mvwprintw(w.win, offset + 4, 1, "%10d", g->lines);

	mvwaddstr(w.win, offset + 6, 1, "Level");
	mvwprintw(w.win, offset + 7, 1, "%10d", g->level);

	wrefresh(w.win);
}

void engine_draw_info(game_s* g)
{
	window_s w = engine.screen.info;

	werase(w.win);

	/* Format: Wed Jun 30 21:49:08 1993\n */
	mvwaddstr(w.win, 0, 0, "yetris v0.5");
	mvwaddstr(w.win, 1, 0, "('yetris -h' for info)");
	mvwaddstr(w.win, 7, 0, "0:00");
	mvwprintw(w.win, 9, 0, "Speed: %d", g->speed);


	/* This is a little hack to display the time onscreen.
	 * It's ugly as hell, but I had to make it */
	time_t cur_time;
	time(&cur_time);
	mvwprintw(w.win, w.height - 1, 15, "%.8s", (ctime(&cur_time) + 11));

	wrefresh(w.win);
}

/** Calls all drawing routines in order */
void engine_draw(game_s* g)
{
	WINDOW* w = engine.screen.board.win;
	werase(w);

	engine_draw_board(&(g->board));
	engine_draw_piece(&(g->piece_ghost), w);
	engine_draw_piece(&(g->piece_current), w);
	engine_draw_next_pieces(g);
	engine_draw_hold(g);
	engine_draw_score(g);
	engine_draw_info(g);

	wrefresh(w);
}

/** Returns the color pair associated with #color.
 *  If #is_bold is true, will make the color brighter.
 */
int engine_get_color(color_e color, bool is_bold)
{
	int col = COLOR_PAIR(color);
//	if (is_bold)
	/* TRY TO MAKE THIS WORK */
		col = col | A_INVIS;
	return col;
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
				b->block[i][j].color = engine_get_color(BLACK_WHITE, false);
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
	fflush(stdin); /* discard any characters pressed until now */
	nodelay(stdscr, FALSE);
	getch();
	nodelay(stdscr, TRUE);
}


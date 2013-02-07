

#include <stdlib.h>
#include <ncurses.h>

#include <sys/time.h>  /* select() */
#include <sys/types.h>
#include <unistd.h>
#include "engine.h"

/* /\** Definitions for the input keys -- arbitrary numbers *\/ */
/* enum Input { RIGHT=666, DOWN,  LEFT, ROTATE, ROTATE_BACKW, */
/*              DROP,      PAUSE, QUIT, ERROR,  NONE }; */
/* typedef enum Input Input; */


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
	getmaxyx (stdscr, current_height, current_width);

	if ((current_width  < engine.screen.width) ||
	    (current_height < engine.screen.height))
	{
		fprintf(stderr, "Your console screen is smaller than %dx%d\n",
		                 engine.screen.width, engine.screen.height);
		fprintf(stderr, "Please resize your window and try again\n");
		endwin();
		exit(EXIT_FAILURE);
	}

	raw ();       /* Character input doesnt require the <enter> key anymore */
	curs_set (0); /* Makes the blinking cursor invisible */
	noecho ();    /* Wont print the keys received through input */
	nodelay (stdscr, TRUE); /* Wont wait for input */
	keypad (stdscr, TRUE);  /* Support for extra keys (life F1, F2, ... ) */
	refresh ();   /* Refresh the screen (prints whats in the screen buffer) */
}

int engine_windows_init()
{
	WINDOW* w = NULL;

	w = newwin(24, 80, 0, 0);
	box(w, ACS_VLINE, ACS_HLINE);
//	wbkgd(w, '*');
	wrefresh(w);
	engine.screen.main = w;

	w = derwin(engine.screen.main, 22, 11 * 2, 1, 2);
	wborder(w, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(w);
	engine.screen.left = w;

	w = derwin(engine.screen.main, 22, 6 * 2, 1, 25);
	wborder(w, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(w);
	engine.screen.middle = w;

	w = derwin(engine.screen.main, 22, 40, 1, 38);
	wborder(w, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(w);
	engine.screen.right = w;

	//////////
	w = derwin(engine.screen.middle, 5, 5 * 2, 1, 1);
	mvwhline(w, 4, 0, '-', 10);
	wrefresh(w);
	engine.screen.next[0] = w;

	w = derwin(engine.screen.middle, 5, 5 * 2, 6, 1);
	wrefresh(w);
	engine.screen.next[1] = w;

	w = derwin(engine.screen.middle, 5, 5 * 2, 11, 1);
	wrefresh(w);
	engine.screen.next[2] = w;

	w = derwin(engine.screen.middle, 5, 5 * 2, 16, 1);
	wrefresh(w);
	engine.screen.next[3] = w;
    //////////

	w = derwin(engine.screen.left, 20, 10 * 2, 1, 1);
	wrefresh(w);
	engine.screen.board = w;

	w = derwin(engine.screen.right, 20, 36, 1, 2);
	wrefresh(w);
	engine.screen.info = w;

	w = engine.screen.info;
	mvwaddstr(w, 1, 1, "yetris v0.5");
	mvwaddstr(w, 3, 5, "Ahh yeah");
	mvwaddstr(w, 4, 5, "This is awesome, everything's fine! "
		               "you see, this is a multilined string "
		               "and i'm still able to print it aligned");
	wrefresh(w);
}

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
		engine.input.down   = 's';
		engine.input.right  = 'd';
		engine.input.left   = 'a';
		engine.input.rotate = 'w';
		engine.input.rotate_backw = 'e';
		engine.input.drop   = ' ';
		engine.input.pause  = 'p';
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



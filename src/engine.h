#ifndef ENGINE_H_DEFINED
#define ENGINE_H_DEFINED

#include <stdlib.h>
#include <ncurses.h>

#define NUMBER_OF_KEYS 8

/** This holds the game keymap. */
typedef struct input_s
{
	char keys[NUMBER_OF_KEYS];
	int  none;
	int  left;
	int  right;
	int  rotate;
	int  down;
	int  drop;
	int  rotate_backw;
	int  quit;
	int  pause;
} input_s;

/** Info about the console screen */
typedef struct screen_s
{
	int width;
	int height;
	WINDOW* main;
	WINDOW* left;
	WINDOW* middle;
	WINDOW* right;
	WINDOW* board;
	WINDOW* info;
	WINDOW* next[4];
} screen_s;

/** Container for all info about the game engine */
typedef struct engine_s
{
	input_s  input;
	screen_s screen;
} engine_s;

engine_s engine;

/** Defining the game color pairs (background_foreground) -- arbitrary numbers */
typedef enum { BLACK_WHITE = 1, WHITE_BLACK,
               BLACK_CYAN,      CYAN_BLACK,
               BLACK_BLUE,      BLUE_BLACK,
               BLACK_RED,       RED_BLACK,
               BLACK_YELLOW,    YELLOW_BLACK,
               BLACK_MAGENTA,   MAGENTA_BLACK,
               BLACK_GREEN,     GREEN_BLACK} color_e;

int engine_screen_init(int width, int height);
int engine_windows_init();
int engine_init();
int block_signals();
int restore_signals();
void engine_exit();
int engine_keymap(char keymap[]);
int engine_get_input(int delay_ms);


#endif /* ENGINE_H_DEFINED */



#ifndef WINDOW_H_DEFINED
#define WINDOW_H_DEFINED

#include <ncurses.h>

/**
 * Stores information about a ncurses' window.
 */
typedef struct window_s
{
	/** nCurses' internal representation of a screen */
	WINDOW* win;

	short x;
	short y;
	short width;
	short height;

} window_s;

/**
 * This creates a sub window based on #main.
 *
 * The new window will have #width and #height and
 * it's #x and #y will be relative to #main.
 */
window_s new_sub_win_from(WINDOW* main, int x, int y, int width, int height);

/**
 * Draws fancy borders on window #win.
 */
void window_fancy_borders(WINDOW* win);

/**
 * Draws normal borders on window #win.
 */
void window_normal_borders(WINDOW* win);

#endif /* WINDOW_H_DEFINED */


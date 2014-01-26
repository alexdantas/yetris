
#ifndef LAYOUT_H_DEFINED
#define LAYOUT_H_DEFINED

#include "globals.h"
#include "window.h"

/**
 * All screens of the game.
 * Mostly ncurses' specific stuff.
 */
typedef struct layout_s
{
	/** Global window width */
	short width;

	/** Global window height */
	short height;

	/** Layout width */
	short original_width;

	/** Layout height */
	short original_height;

	/** Main window */
	window_s main;

	window_s leftmost;
	window_s middle_left;
	window_s middle_right;
	window_s rightmost;
	window_s next_container;
	window_s board;
	window_s info;
	window_s next[NEXT_PIECES_NO];
	window_s hold;
	window_s leftmost_container;
	window_s score;
	window_s help_container;
	window_s help;
	window_s hscores_container;
	window_s hscores;
	window_s input_container;
	window_s input;

} layout_s;

/**
 * Creates the layout - all windows inside the game,
 * with their placements.
 *
 * #width and #height are the minimum sizes we expect
 * the game to have.
 * Layout should be responsive to them.
 */
void layout_init(int width, int height);

/**
 * Creates all the windows on the game.
 */
void layout_windows_init();

#endif /* LAYOUT_H_DEFINED */


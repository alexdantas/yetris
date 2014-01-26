#include "window.h"
#include "color.h"

window_s new_sub_win_from(WINDOW* main, int x, int y, int width, int height)
{
	window_s w;
	w.width  = width;
	w.height = height;
	w.x      = x;
	w.y      = y;
	w.win    = derwin(main, height, width, y, x);
	wnoutrefresh(w.win);
	return w;
}

void window_fancy_borders(WINDOW* win)
{
	wborder(win, ACS_VLINE|color_pair(COLOR_WHITE, COLOR_DEFAULT, false),
	        ACS_VLINE|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD,
	        ACS_HLINE|color_pair(COLOR_WHITE, COLOR_DEFAULT, false),
	        ACS_HLINE|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD,
	        ACS_ULCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false)|A_BOLD,
	        ACS_URCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false),
	        ACS_LLCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false),
	        ACS_LRCORNER|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD);
}

void window_normal_borders(WINDOW* win)
{
	wattrset(win, color_pair(COLOR_BLACK, COLOR_DEFAULT, true));
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
}


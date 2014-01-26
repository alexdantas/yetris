#include "layout.h"
#include "engine.h"

/* dag-nabbit, PDCurses (windows) doesnt have 'mvwhline' */
#if OS_IS_WINDOWS
#define mvwhline my_mvwhline
#endif

/**
 * PDCurses (on Windows) doesn't have this function, so I need
 * to re-implement it.
 *
 * @todo implement more features - see 'man mvwhline'
 */
void my_mvwhline(WINDOW* win, int y, int x, chtype ch, int num)
{
	int i;
	for (i = 0; i < num; i++)
		mvwaddch(win, y, (x + i), ch);
}

void layout_init(int width, int height)
{
	/* Gets the current width and height */
	int current_height, current_width;
	getmaxyx(stdscr, current_height, current_width);

	if ((current_width  < width) ||
	    (current_height < height))
	{
		endwin();
		fprintf(stderr,
		        "Error! Your console screen is smaller than %dx%d\n"
		        "Please resize your window and try again\n",
		        width, height);

		exit(EXIT_FAILURE);
	}

	engine.layout.width  = current_width;
	engine.layout.height = current_height;

	engine.layout.original_width  = width;
	engine.layout.original_height = height;

	layout_windows_init();
}
void layout_windows_init()
{
	/* now we'll start all the windows inside the layout */

	window_s  w;
	layout_s* s = &(engine.layout);

	int main_x = 0;
	int main_y = 0;

	if (global.screen_center_horizontally)
		main_x = engine.layout.width/2 - engine.layout.original_width/2;

	if (global.screen_center_vertically)
		main_y = engine.layout.height/2 - engine.layout.original_height/2;

	/* main window, wrapper of all others */
	w.width  = engine.layout.original_width;
	w.height = engine.layout.original_height;
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

	wnoutrefresh(w.win);
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
			mvwaddch(w.win, 5, 0, ACS_LLCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
			my_mvwhline(w.win, 5, 1, ACS_HLINE|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD, w.width - 2);
			mvwaddch(w.win, 5, w.width - 1, ACS_LRCORNER|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD);

			/* making the bottom line between hold and score windows */
			mvwaddch(w.win, 6, 0, ACS_ULCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false)|A_BOLD);
			my_mvwhline(w.win, 6, 1, ACS_HLINE|color_pair(COLOR_WHITE, COLOR_DEFAULT, false), w.width - 2);
			mvwaddch(w.win, 6, w.width - 1, ACS_URCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
		}

	}
	else
	{
		window_normal_borders(w.win);
		wattrset(w.win, color_pair(COLOR_BLACK, COLOR_DEFAULT, true));
		mvwhline(w.win, 5, 1, '-', w.width - 2);
	}

	wnoutrefresh(w.win);
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
	wnoutrefresh(w.win);
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
		mvwaddch(w.win, 3, 0, ACS_LLCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
		mvwhline(w.win, 3, 1, ACS_HLINE|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD, w.width - 2);
		mvwaddch(w.win, 3, w.width - 1, ACS_LRCORNER|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD);

		/* making the bottom line between 1st next and the rest */
		mvwaddch(w.win, 4, 0, ACS_ULCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false)|A_BOLD);
		mvwhline(w.win, 4, 1, ACS_HLINE|color_pair(COLOR_WHITE, COLOR_DEFAULT, false), w.width - 2);
		mvwaddch(w.win, 4, w.width - 1, ACS_URCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));

	}
	else
	{
		window_normal_borders(w.win);
		wattrset(w.win, color_pair(COLOR_BLACK, COLOR_DEFAULT, true));
		mvwhline(w.win, 3, 1, '-', w.width - 2);
	}
	wnoutrefresh(w.win);
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
	wnoutrefresh(w.win);
	s->rightmost = w;

	/* next pieces */
	w.width  = s->middle_right.width  - 2;
	w.height = s->middle_right.height - 2;
	w.x      = 1;
	w.y      = 1;
	w.win    = derwin(s->middle_right.win, w.height, w.width, w.y, w.x);
	wnoutrefresh(w.win);
	s->next_container = w;

	/* first next piece */
	w.width  = s->next_container.width;
	w.height = 2;
	w.x      = 0;
	w.y      = 0;
	w.win    = derwin(s->next_container.win, w.height, w.width, w.y, w.x);
	wnoutrefresh(w.win);
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
		wnoutrefresh(w.win);
		s->next[i] = w;
	}

	s->board = new_sub_win_from(s->middle_left.win,
	                            1,
	                            1,
	                            s->middle_left.width - 2,
	                            s->middle_left.height - 2);

	s->info = new_sub_win_from(s->rightmost.win,
							   2,
	                           1,
	                           s->rightmost.width - 4,
	                           s->rightmost.height - 2);

	s->leftmost_container = new_sub_win_from(s->leftmost.win,
	                                         1,
	                                         1,
	                                         s->leftmost.width - 2,
	                                         s->leftmost.height - 2);

	s->hold = new_sub_win_from(s->leftmost_container.win,
	                           0,
	                           0,
	                           s->leftmost_container.width,
	                           4);

	s->score = new_sub_win_from(s->leftmost_container.win,
	                            0,
	                            s->hold.y + s->hold.height + 2,
	                            s->leftmost_container.width,
	                            s->leftmost_container.height - (s->hold.height) - 2);

	/* w.width  = s->leftmost_container.width; */
	/* w.height = s->leftmost_container.height - (s->hold.height) - 2; */
	/* w.x      = 0; */
	/* w.y      = s->hold.y + s->hold.height + 2; */
	/* w.win    = derwin(s->leftmost_container.win, w.height, w.width, w.y, w.x); */
	/* wnoutrefresh(w.win); */
	/* s->score = w; */

	w = s->info;
	wattrset(w.win, color_pair(COLOR_WHITE, COLOR_DEFAULT, true));
	mvwaddstr(w.win, w.height - 1, 16 , "Loading");
	wnoutrefresh(w.win);
}


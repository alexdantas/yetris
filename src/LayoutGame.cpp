#include "LayoutGame.hpp"
#include "Ncurses.hpp"
#include <iostream>
#include <cstdlib>				// exit()

LayoutGame::LayoutGame(int width, int height)
{
	/* Gets the current width and height */
	int current_height, current_width;
	getmaxyx(stdscr, current_height, current_width);

	if ((current_width  < width) ||
	    (current_height < height))
	{
		Ncurses::exit();
		std::cerr << "Error! Your console screen is smaller than"
		          << width << "x" << height << std::endl
		          << "Please resize your window and try again"
		          << std::endl;

		exit(EXIT_FAILURE);
	}

	this->width  = current_width;
	this->height = current_height;

	this->originalWidth  = width;
	this->originalHeight = height;

	this->windowsInit();
}
LayoutGame::~LayoutGame()
{
	this->windowsExit();
}

void LayoutGame::windowsInit()
{
	/* now we'll start all the windows inside the layout */

	int main_x = 0;
	int main_y = 0;

	// if (global.screen_center_horizontally)
	// 	main_x = engine.layout->getW()/2 - engine.layout.original_width/2;

	// if (global.screen_center_vertically)
	// 	main_y = engine.layout->getH()/2 - engine.layout.original_height/2;

	/* main window, wrapper of all others */
	this->main = new Window(main_x,
	                        main_y,
	                        originalWidth,
	                        originalHeight);

	bool show_border = true;
	bool fancy_border = true;

	if (show_border)
	{
		if (fancy_border)
			this->main->borders(Window::BORDER_FANCY);
		else
			this->main->borders(Window::BORDER_REGULAR);
	}

	// continuar substituindo toda window_s por WINDOW
	// pra terminar o layout
	// por fim, testar o layout na main() e commitar
	// dizendo que tem o layout pronto

	this->main->refresh();

	/* leftmost */
	this->leftmost = new Window(this->main,
	                            2,
	                            1,
	                            6 * 2 + 2,
	                            this->main->getH() - 2); // borders

	if (fancy_border)
	{
		this->leftmost->borders(Window::BORDER_FANCY);

		// /* If the player has no hold, doesnt make sense printing these parts */
		// if (global.game_can_hold)
		// {
		// 	/* making the top line between hold and score windows */
		// 	mvwaddch(w.win, 5, 0, ACS_LLCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
		// 	my_mvwhline(w.win, 5, 1, ACS_HLINE|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD, w->getW() - 2);
		// 	mvwaddch(w.win, 5, w->getW() - 1, ACS_LRCORNER|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD);

		// 	/* making the bottom line between hold and score windows */
		// 	mvwaddch(w.win, 6, 0, ACS_ULCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false)|A_BOLD);
		// 	my_mvwhline(w.win, 6, 1, ACS_HLINE|color_pair(COLOR_WHITE, COLOR_DEFAULT, false), w->getW() - 2);
		// 	mvwaddch(w.win, 6, w->getW() - 1, ACS_URCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
		// }
	}
	else
	{
		this->leftmost->borders(Window::BORDER_FANCY);
		this->leftmost->horizontalLine(1,
		                               5,
		                               '-',
		                               this->leftmost->getW() - 2,
		                               Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));
	}
	this->leftmost->refresh();

	/* middle-left */
	this->middle_left = new Window(this->main,
	                               this->leftmost->getX() + this->leftmost->getW() + 1,
	                               1,
	                               10 * 2 + 2,
	                               this->main->getH() - 2); /* borders */

	if (fancy_border)
		this->middle_left->borders(Window::BORDER_FANCY);
	else
		this->middle_left->borders(Window::BORDER_REGULAR);

	this->middle_left->refresh();

	/* middle-right */
	this->middle_right = new Window(this->main,
	                                this->middle_left->getX() + this->middle_left->getW() + 1,
	                                1,
	                                4 * 2 + 2,
	                                this->main->getH() - 2); /* borders */

	if (fancy_border)
	{
		this->middle_right->borders(Window::BORDER_FANCY);

		/* making the top line between 1st next and the rest */
		// mvwaddch(w.win, 3, 0, ACS_LLCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));
		// mvwhline(w.win, 3, 1, ACS_HLINE|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD, w->getW() - 2);
		// mvwaddch(w.win, 3, w->getW() - 1, ACS_LRCORNER|color_pair(COLOR_BLACK, COLOR_DEFAULT, false)|A_BOLD);

		// /* making the bottom line between 1st next and the rest */
		// mvwaddch(w.win, 4, 0, ACS_ULCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false)|A_BOLD);
		// mvwhline(w.win, 4, 1, ACS_HLINE|color_pair(COLOR_WHITE, COLOR_DEFAULT, false), w->getW() - 2);
		// mvwaddch(w.win, 4, w->getW() - 1, ACS_URCORNER|color_pair(COLOR_WHITE, COLOR_DEFAULT, false));

	}
	else
	{
		this->middle_right->borders(Window::BORDER_REGULAR);

		// wattrset(w.win, color_pair(COLOR_BLACK, COLOR_DEFAULT, true));
		// mvwhline(w.win, 3, 1, '-', w->getW() - 2);
	}
	this->middle_right->refresh();

	/* right-most */
	this->rightmost = new Window(this->main,
	                             this->middle_right->getX() + this->middle_right->getW() + 1,
	                             1,
	                             this->main->getW() - (this->middle_right->getX() + this->middle_right->getW()) - 3,
	                             this->main->getH() - 2); /* borders */

	if (fancy_border)
		this->rightmost->borders(Window::BORDER_FANCY);
	else
		this->rightmost->borders(Window::BORDER_REGULAR);

	this->rightmost->refresh();

	this->next_container = new Window(this->middle_right,
	                                  1,
	                                  1,
	                                  this->middle_right->getW()  - 2,
	                                  this->middle_right->getH() - 2);

	this->next_container->refresh();

	/* first next piece */
	this->next[0] = new Window(this->next_container,
	                           0,
	                           0,
	                           this->next_container->getW(),
	                           2);

	this->next[0]->refresh();

	/* the rest */
	int y_offset = 2;
	for (int i = 1; i <= 3/*global.game_next_no*/; i++)
	{
		/* making all the next pieces 1 line lower */
		if (i != 1)
			y_offset = 0;

		this->next[i] = new Window(this->next_container,
		                           0,
		                           this->next[i - 1]->getY() + this->next[i - 1]->getH() + 1 + y_offset,
		                           this->next_container->getW(),
		                           2);

		this->next[i]->refresh();
	}

	this->board = new Window(this->middle_left,
	                         1,
	                         1,
	                         this->middle_left->getW() - 2,
	                         this->middle_left->getH() - 2);

	this->info = new Window(this->rightmost,
	                        2,
	                        1,
	                        this->rightmost->getW() - 4,
	                        this->rightmost->getH() - 2);

	this->leftmost_container = new Window(this->leftmost,
	                                      1,
	                                      1,
	                                      this->leftmost->getW() - 2,
	                                      this->leftmost->getH() - 2);

	this->hold = new Window(this->leftmost_container,
	                        0,
	                        0,
	                        this->leftmost_container->getW(),
	                        4);

	this->score = new Window(this->leftmost_container,
	                      0,
	                      this->hold->getY() + this->hold->getH() + 2,
	                      this->leftmost_container->getW(),
	                      this->leftmost_container->getH() - (this->hold->getH()) - 2);

	/* w->getW()  = s->leftmost_container->getW(); */
	/* w->getH() = s->leftmost_container->getH() - (s->hold->getH()) - 2; */
	/* w->getX()      = 0; */
	/* w->getY()      = s->hold->getY() + s->hold->getH() + 2; */
	/* w.win    = derwin(s->leftmost_container.win, w->getH(), w->getW(), w->getY(), w->getX()); */
	/* wnoutrefresh(w.win); */
	/* s->score = w; */

	this->info->print("Loading", 16, this->info->getH() - 1, Colors::pair(COLOR_WHITE, COLOR_DEFAULT, true));
	this->info->refresh();
}
void LayoutGame::windowsExit()
{

}
void LayoutGame::draw()
{

}



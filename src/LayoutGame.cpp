#include "LayoutGame.hpp"
#include "Ncurses.hpp"
#include "Globals.hpp"
#include <iostream>
#include <cstdlib>				// exit()

LayoutGame::LayoutGame(int width, int height):
	main(NULL),
	leftmost(NULL),
	middle_left(NULL),
	middle_right(NULL),
	rightmost(NULL),
	next_container(NULL),
	board(NULL),
	info(NULL),
	hold(NULL),
	leftmost_container(NULL),
	score(NULL),
	help_container(NULL),
	help(NULL),
	hscores_container(NULL),
	hscores(NULL),
	input_container(NULL),
	input(NULL)
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
	// We'll start all the windows inside the Layout

	int main_x = 0;
	int main_y = 0;

	if (Globals::Screen::center_horizontally)
		main_x = this->width/2 - this->originalWidth/2;

	if (Globals::Screen::center_vertically)
		main_y = this->height/2 - this->originalHeight/2;

	// Main window, wrapper of all others
	this->main = new Window(main_x,
	                        main_y,
	                        originalWidth,
	                        originalHeight);

	if (Globals::Screen::show_borders)
	{
		this->main->borders(Globals::Screen::fancy_borders ?
		                    Window::BORDER_FANCY :
		                    Window::BORDER_REGULAR);
	}
	this->main->refresh();

	// Leftmost window
	this->leftmost = new Window(this->main,
	                            0,
	                            0,
	                            6*2 + 2,
	                            0);

	if (Globals::Screen::fancy_borders)
	{
		this->leftmost->borders(Window::BORDER_FANCY);

		// If the player has no hold,
		// doesn't make sense printing these parts
//		if (global.game_can_hold)
		{
			// making the top line between
			// hold and score windows
			this->leftmost->printChar(0,
			                          5,
			                          ACS_LLCORNER,
			                          Colors::pair(COLOR_WHITE, COLOR_DEFAULT));
			this->leftmost->horizontalLine(1,
			                               5,
			                               ACS_HLINE,
			                               this->leftmost->getW() - 2,
			                               Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));

			this->leftmost->printChar(this->leftmost->getW() - 1,
			                          5,
			                          ACS_LRCORNER,
			                          Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));

			// Making the bottom line
			// between hold and score windows
			this->leftmost->printChar(0,
			                         6,
			                         ACS_ULCORNER,
			                         Colors::pair(COLOR_WHITE, COLOR_DEFAULT, true));

			this->leftmost->horizontalLine(1,
			                               6,
			                               ACS_HLINE,
			                               this->leftmost->getW() - 2,
			                               Colors::pair(COLOR_WHITE, COLOR_DEFAULT, false));

			this->leftmost->printChar(this->leftmost->getW() - 1,
			                          6,
			                          ACS_URCORNER,
			                          Colors::pair(COLOR_WHITE, COLOR_DEFAULT, false));
		}
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

	// Middle-left
	this->middle_left = new Window(this->main,
	                               this->leftmost->getX() + this->leftmost->getW(),
	                               1,
	                               10 * 2 + 2,
	                               0);

	if (Globals::Screen::show_borders)
	{
		this->middle_left->borders(Globals::Screen::fancy_borders ?
		                           Window::BORDER_FANCY :
		                           Window::BORDER_REGULAR);
	}
	this->middle_left->refresh();

	// Middle-right
	this->middle_right = new Window(this->main,
	                                this->middle_left->getX() + this->middle_left->getW(),
	                                1,
	                                4 * 2 + 2,
	                                0);

	if (Globals::Screen::fancy_borders)
	{
		this->middle_right->borders(Window::BORDER_FANCY);

		// Making the top line between 1st next and the rest
		this->middle_right->printChar(3,
		                              0,
		                              ACS_LLCORNER,
		                              Colors::pair(COLOR_WHITE, COLOR_DEFAULT, false));

		this->middle_right->horizontalLine(1,
		                                   3,
		                                   ACS_HLINE,
		                                   this->middle_right->getW() - 2,
		                                   Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));

		this->middle_right->printChar(this->middle_right->getW() - 1,
		                              3,
		                              ACS_LRCORNER,
		                              Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));

		// /* making the bottom line between 1st next and the rest */
		// printChar(w.win, 4, 0, ACS_ULCORNER|Colors::pair(COLOR_WHITE, COLOR_DEFAULT, false)|A_BOLD);
		// mvwhline(w.win, 4, 1, ACS_HLINE|Colors::pair(COLOR_WHITE, COLOR_DEFAULT, false), w->getW() - 2);
		// printChar(w.win, 4, w->getW() - 1, ACS_URCORNER|Colors::pair(COLOR_WHITE, COLOR_DEFAULT, false));

	}
	else
	{
		this->middle_right->borders(Window::BORDER_REGULAR);

		// wattrset(w.win, Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));
		// mvwhline(w.win, 3, 1, '-', w->getW() - 2);
	}
	this->middle_right->refresh();

	/* right-most */
	this->rightmost = new Window(this->main,
	                             this->middle_right->getX() + this->middle_right->getW(),
	                             1,
	                             this->main->getW() - (this->middle_right->getX() + this->middle_right->getW()) - 1,
	                             0);

	if (Globals::Screen::show_borders)
	{
		this->rightmost->borders(Globals::Screen::fancy_borders ?
		                         Window::BORDER_FANCY :
		                         Window::BORDER_REGULAR);
	}
	this->rightmost->refresh();

	// Next Piece Container
	this->next_container = new Window(this->middle_right,
	                                  1,
	                                  1,
	                                  0,
	                                  0);
	this->next_container->refresh();

	// First next piece
	this->next.resize(Globals::Game::next_pieces);
	this->next[0] = new Window(this->next_container,
	                           0,
	                           0,
	                           this->next_container->getW(),
	                           2);
	this->next[0]->refresh();

	// The rest of the next pieces
	int y_offset = 2;

	for (unsigned int i = 1; i < this->next.size(); i++)
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
	                         0,
	                         0);

	this->info = new Window(this->rightmost,
	                        2,
	                        1,
	                        this->rightmost->getW() - 4,
	                        0);

	this->leftmost_container = new Window(this->leftmost,
	                                      1,
	                                      1,
	                                      0,
	                                      0);

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

	this->info->print("Loading",
	                  0,
	                  this->info->getH() - 1,
	                  Colors::pair(COLOR_WHITE, COLOR_DEFAULT, true));
	this->info->refresh();
}
void LayoutGame::windowsExit()
{
#define SAFE_DELETE(n) \
	{				   \
		if (n)		   \
		{			   \
			delete(n); \
			n = NULL;  \
		}			   \
	}

	SAFE_DELETE(this->main);

	SAFE_DELETE(this->leftmost);
	SAFE_DELETE(this->middle_left);
	SAFE_DELETE(this->middle_right);
	SAFE_DELETE(this->rightmost);
	SAFE_DELETE(this->next_container);
	SAFE_DELETE(this->board);
	SAFE_DELETE(this->info);

	for (unsigned int i = 0; i < this->next.size(); i++)
		SAFE_DELETE(this->next[i]);

	this->next.clear();

	SAFE_DELETE(this->hold);
	SAFE_DELETE(this->leftmost_container);
	SAFE_DELETE(this->score);
	SAFE_DELETE(this->help_container);
	SAFE_DELETE(this->help);
	SAFE_DELETE(this->hscores_container);
	SAFE_DELETE(this->hscores);
	SAFE_DELETE(this->input_container);
	SAFE_DELETE(this->input);
}
void LayoutGame::draw()
{

}



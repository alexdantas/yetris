#include <Interface/LayoutGame.hpp>
#include <Interface/Ncurses.hpp>

#include <iostream>
#include <cstdlib>				// exit()

LayoutGame::LayoutGame(int width, int height):
	main(NULL),
	board(NULL),
	hold(NULL),
	info(NULL)
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
}
LayoutGame::~LayoutGame()
{ }


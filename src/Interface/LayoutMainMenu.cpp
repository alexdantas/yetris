#include <Interface/LayoutMainMenu.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>

#include <iostream>

LayoutMainMenu::LayoutMainMenu(int width, int height):
	main(NULL)
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
LayoutMainMenu::~LayoutMainMenu()
{ }
void LayoutMainMenu::windowsInit()
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
}
void LayoutMainMenu::windowsExit()
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
}


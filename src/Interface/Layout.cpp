#include <Interface/Layout.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>

#include <iostream>
#include <cstdlib>

int Layout::screenWidth  = 0;
int Layout::screenHeight = 0;

Layout::Layout(int width, int height):
	main(nullptr)
{
	// Gets the current width and height of the whole terminal.
	int current_height, current_width;
	getmaxyx(stdscr, current_height, current_width);

	if ((current_width < width) || (current_height < height))
	{
		Ncurses::exit();
		std::cerr << "Error! Your console screen is smaller than"
		          << width << "x" << height << std::endl
		          << "Please resize your window and try again"
		          << std::endl;

		exit(EXIT_FAILURE);
	}

	// Sets global info
	Layout::screenWidth  = current_width;
	Layout::screenHeight = current_height;

	// Creating the main window for this layout.
	int main_x = 0;
	if (Globals::Profiles::current->settings.screen.center_horizontally)
		main_x = current_width/2 - width/2;

	int main_y = 0;
	if (Globals::Profiles::current->settings.screen.center_vertically)
		main_y = current_height/2 - height/2;

	this->main = new Window(main_x, main_y, width, height);

	if (Globals::Profiles::current->settings.screen.show_borders)
	{
		this->main->borders(Globals::Profiles::current->settings.screen.fancy_borders ?
		                    Window::BORDER_FANCY :
		                    Window::BORDER_REGULAR);
	}

	this->main->refresh();
}
Layout::~Layout()
{
	delete this->main;
}
void Layout::draw()
{
	// When subclassing, make sure to implement this!
}


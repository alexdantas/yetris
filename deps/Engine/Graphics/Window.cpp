#include <Engine/Graphics/Window.hpp>
#include <Engine/EngineGlobals.hpp>

#include <sstream>				// stringstream
#include <iostream>

Window::Window(int x, int y, int w, int h):
	win(NULL),
	x(x),
	y(y),
	width(w),
	height(h),
	borderType(BORDER_NONE),
	topLeftTitle(""),
	topRightTitle(""),
	bottomLeftTitle(""),
	bottomRightTitle("")
{
	this->win = newwin(height, width, y, x);

	if (! this->win)
		throw "Could not create Ncurses Window";

	this->setBorders();
}
Window::Window(Window* parent, int x, int y, int width, int height):
	win(NULL),
	borderType(BORDER_NONE),
	topLeftTitle(""),
	topRightTitle(""),
	bottomLeftTitle(""),
	bottomRightTitle("")
{
	// By sending any parameter as 0, we want it to expand
	// until possible.
	// Let's expand based if the parent window has borders
	// or not.
	if (parent->borderType == BORDER_NONE)
	{
		if (width  == 0) width  = parent->width;
		if (height == 0) height = parent->height;
	}
	else
	{
		// Has borders
		if (x == 0) x = 1;
		if (y == 0) y = 1;

		if (width  == 0) width  = parent->width  - 2;
		if (height == 0) height = parent->height - 2;
	}

	this->x = x;
	this->y = y;
	this->width  = width;
	this->height = height;

	// Creates a subwindow
	this->win = derwin(parent->win, height, width, y, x);
	if (! this->win)
		throw "Could not create Ncurses Window";

	this->setBorders();
}
Window::~Window()
{
	if (this->win)
		delwin(this->win);
}
void Window::resize(int w, int h)
{
	wresize(this->win, h, w);

	this->width  = w;
	this->height = h;
}
void Window::print(std::string str, int x, int y, ColorPair pair)
{
	Colors::pairActivate(this->win, pair);

	if (! str.empty())
		// Ncurses has this strange habit of
		// switching X and Y coordinates...
		mvwaddstr(this->win, y, x, str.c_str());
}
void Window::print(std::vector<std::string> lines, int x, int y, ColorPair pair)
{
	for (size_t i = 0; i < lines.size(); i++)
		this->print(lines[i], x, (y + i), pair);
}
void Window::printChar(int c, int x, int y, ColorPair pair)
{
	Colors::pairActivate(this->win, pair);

	// Ncurses has this strange habit of
	// switching X and Y coordinates...
	mvwaddch(this->win, y, x, c);
}
void Window::setBackground(chtype ch, ColorPair pair)
{
	wbkgd(this->win, (ch | pair.ncurses_pair));
}
void Window::refresh()
{
	// Previously it was:
	//
	//wrefresh(this->win);
	//
	// I've changed all calls to wrefresh() to wnoutrefresh
	// because when I have several WINDOW*, it gets heavy
	// to do the former.
	//
	// As a tradeoff, I need to call `refresh()` at the end
	// of every draw cycle.
	//
	wnoutrefresh(this->win);
}
void Window::clear()
{
	werase(this->win);

	// Redrawing borders if existing
	if (this->borderType != BORDER_NONE)
		this->borders(this->borderType);

	// Now, to the titles!
	if (! this->topLeftTitle.empty())
	{
		this->print(this->topLeftTitle,
		            1, 0,
		            EngineGlobals::Theme::hilite_hilite_text);
	}
	if (! this->bottomLeftTitle.empty())
	{
		this->print(this->bottomLeftTitle,
		            0, this->getH() - 1,
		            EngineGlobals::Theme::hilite_hilite_text);
	}
	if (! this->topRightTitle.empty())
	{
		int x = (this->getW() - 1);
		int w = this->topRightTitle.size();

		this->print(this->topRightTitle,
		            x - w, 0,
		            EngineGlobals::Theme::hilite_hilite_text);
	}
	if (! this->bottomRightTitle.empty())
	{
		int x = (this->getW() - 1);
		int w = this->bottomRightTitle.size();

		this->print(this->bottomRightTitle,
		            x - w, this->getH() - 1,
		            EngineGlobals::Theme::hilite_hilite_text);
	}
}
int Window::getW() const
{
	return this->width;
}
int Window::getH() const
{
	return this->height;
}
int Window::getX() const
{
	return this->x;
}
int Window::getY() const
{
	return this->y;
}
void Window::borders(BorderType type)
{
	this->borderType = type;

	if (type == Window::BORDER_NONE)
		return;

	if (type == Window::BORDER_FANCY)
	{
		// Making shadows with Theme colors
		wborder(this->win,
		        ACS_VLINE    | EngineGlobals::Theme::dim_text.ncurses_pair,
		        ACS_VLINE    | EngineGlobals::Theme::dim_dim_text.ncurses_pair,
		        ACS_HLINE    | EngineGlobals::Theme::dim_text.ncurses_pair,
		        ACS_HLINE    | EngineGlobals::Theme::dim_dim_text.ncurses_pair,
		        ACS_ULCORNER | EngineGlobals::Theme::text.ncurses_pair,
		        ACS_URCORNER | EngineGlobals::Theme::dim_text.ncurses_pair,
		        ACS_LLCORNER | EngineGlobals::Theme::dim_text.ncurses_pair,
		        ACS_LRCORNER | EngineGlobals::Theme::dim_dim_text.ncurses_pair);
	}
	else if (type == Window::BORDER_REGULAR)
	{
		Colors::pairActivate(this->win, EngineGlobals::Theme::dim_text);
		wborder(this->win, '|', '|', '-', '-', '+', '+', '+', '+');
	}
}
void Window::setBorders()
{
	if (EngineGlobals::Screen::show_borders)
	{
		this->borders(EngineGlobals::Screen::fancy_borders ?
		              Window::BORDER_FANCY :
		              Window::BORDER_REGULAR);
	}
}
void Window::horizontalLine(int x, int y, int c, int width, ColorPair pair)
{
	Colors::pairActivate(this->win, pair);
	mvwhline(this->win, y, x, c, width);
}
void Window::setTitle(std::string title, WindowTitlePosition position)
{
	switch (position)
	{
	case TOP_LEFT:     this->topLeftTitle     = title; break;
	case TOP_RIGHT:    this->topRightTitle    = title; break;
	case BOTTOM_LEFT:  this->bottomLeftTitle  = title; break;
	case BOTTOM_RIGHT: this->bottomRightTitle = title; break;
	default: return;
	}
}


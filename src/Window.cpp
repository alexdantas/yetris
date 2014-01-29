#include "Window.hpp"

Window::Window(int x, int y, int w, int h):
	win(NULL),
	error(false),
	x(x),
	y(y),
	width(w),
	height(h),
	borderType(BORDER_NONE)
{
	this->win = newwin(height, width, y, x);

	if (!this->win)
		this->error = true;
}
Window::Window(Window* parent, int x, int y, int width, int height):
	win(NULL),
	error(false),
	x(x),
	y(y),
	width(width),
	height(height)
{
	this->win = derwin(parent->win, height, width, y, x);
	if (!win)
		this->error = true;
}
Window::~Window()
{
	if (this->win)
		delwin(this->win);
}
bool Window::isValid()
{
	return !(this->error);
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

	mvwaddstr(this->win, y, x, str.c_str());
}
void Window::setBackground(chtype ch, unsigned long pair)
{
	wbkgd(this->win, ch | pair);
}
void Window::refresh()
{
	wrefresh(this->win);
	//wnoutrefresh?
}
void Window::clear()
{
	werase(this->win);

	// Redrawing borders if existing
	if (this->borderType != BORDER_NONE)
		this->borders(this->borderType);
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

	if (type == BORDER_NONE)
		return;

	if (type == BORDER_FANCY)
	{
		wborder(this->win,
		        ACS_VLINE|Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
		        ACS_VLINE|Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true),
		        ACS_HLINE|Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
		        ACS_HLINE|Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true),
		        ACS_ULCORNER|Colors::pair(COLOR_WHITE, COLOR_DEFAULT, true),
		        ACS_URCORNER|Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
		        ACS_LLCORNER|Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
		        ACS_LRCORNER|Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));
	}
	else if (type == BORDER_REGULAR)
	{
		wattrset(this->win, Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));
		wborder(this->win, '|', '|', '-', '-', '+', '+', '+', '+');
	}
}
void Window::horizontalLine(int x, int y, chtype c, int width, ColorPair pair)
{
	Colors::pairActivate(this->win, pair);
	mvwhline(this->win, y, x, c, width);
}


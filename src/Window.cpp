#include "Window.hpp"


Window::Window(int x, int y, int w, int h):
	win(NULL),
	error(false),
	x(x),
	y(y),
	width(w),
	height(h)
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
void Window::print(std::string str, int x, int y)
{
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


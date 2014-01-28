#ifndef WINDOW_H_DEFINED
#define WINDOW_H_DEFINED

#include <string>
#include <ncurses.h>

/// A segment of the terminal screen (2D char matrix).
///
class Window
{
public:
	Window(int x, int y, int w, int h);

	Window(Window* parent, int x, int y, int width, int height);

	virtual ~Window();

	virtual void resize(int w, int h);

	bool isValid();

	void print(std::string str, int x, int y);

	void setBackground(chtype ch, unsigned long pair);

	void refresh();

	void clear();

	int getW() const;
	int getH() const;
	int getX() const;
	int getY() const;

protected:
	/// Ncurses' internal data structure.
	WINDOW* win;

	/// Tells if we've got some initialization error.
	bool error;

	int x;
	int y;
	int width;
	int height;
};

#endif //WINDOW_H_DEFINED


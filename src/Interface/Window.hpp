#ifndef WINDOW_H_DEFINED
#define WINDOW_H_DEFINED

#include <string>
#include <ncurses.h>

#include <Interface/Colors.hpp>

/// A segment of the terminal screen (2D char matrix).
///
class Window
{
public:
	enum BorderType
	{
		BORDER_NONE, BORDER_REGULAR, BORDER_FANCY
	};

	Window(int x, int y, int w, int h);

	Window(Window* parent, int x, int y, int width, int height);

	virtual ~Window();

	virtual void resize(int w, int h);

	bool isValid();

	void print(std::string str, int x, int y, ColorPair pair=0);
	void printChar(int c, int x, int y, ColorPair pair=0);

	void setBackground(chtype ch, unsigned long pair);

	void refresh();

	void clear();

	int getW() const;
	int getH() const;
	int getX() const;
	int getY() const;

	void borders(BorderType type);

	void horizontalLine(int x, int y, int c, int width, ColorPair pair);

protected:
	/// Ncurses' internal data structure.
	WINDOW* win;

	/// Tells if we've got some initialization error.
	bool error;

	int x;
	int y;
	int width;
	int height;

	BorderType borderType;
};

#endif //WINDOW_H_DEFINED


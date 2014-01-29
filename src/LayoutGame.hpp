#ifndef LAYOUTGAME_H_DEFINED
#define LAYOUTGAME_H_DEFINED

#include "Layout.hpp"

#define NEXT_PIECES_NO 8

/// All the windows on the game and how they're displayed.
///
class LayoutGame: public Layout
{
public:

	LayoutGame(int width, int height);
	virtual ~LayoutGame();

	void windowsInit();
	void windowsExit();

	void draw();

private:

	/// Global window width
	short width;

	/// Global window height
	short height;

	/// Layout width
	short originalWidth;

	/// Layout height
	short originalHeight;

	/// Main window
	Window* main;

	Window* leftmost;
	Window* middle_left;
	Window* middle_right;
	Window* rightmost;
	Window* next_container;
	Window* board;
	Window* info;
	Window* next[NEXT_PIECES_NO];
	Window* hold;
	Window* leftmost_container;
	Window* score;
	Window* help_container;
	Window* help;
	Window* hscores_container;
	Window* hscores;
	Window* input_container;
	Window* input;
};

#endif //LAYOUTGAME_H_DEFINED


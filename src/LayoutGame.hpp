#ifndef LAYOUTGAME_H_DEFINED
#define LAYOUTGAME_H_DEFINED

#include "Layout.hpp"
#include <vector>

/// Abstract interface on how the things on the GameModes
/// are displayed.
///
/// You see, this interface has the main Windows where
/// all the things that compose a GameMode are shown.
///
/// To create alternative Layouts, inherit this class and
/// place things on your own.
///
class LayoutGame: public Layout
{
public:
	LayoutGame(int width, int height);
	virtual ~LayoutGame();

	void draw() {};

	/// Terminal full width.
	short width;

	/// Terminal full height.
	short height;

	/// Layout minimum width.
	short originalWidth;

	/// Layout minimum height.
	short originalHeight;

	/// The main window, where all the others are inside.
	Window* main;

	/// Where the game Board is shown.
	Window* board;

	/// Where the hold piece is displayed.
	Window* hold;

	/// Where some info on the current game is shown.
	Window* info;
};

#endif //LAYOUTGAME_H_DEFINED


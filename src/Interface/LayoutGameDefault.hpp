#ifndef LAYOUTGAMEDEFAULT_H_DEFINED
#define LAYOUTGAMEDEFAULT_H_DEFINED

#include <vector>

#include <Interface/LayoutGame.hpp>

/// The default game layout, suited for most
/// single-player game modes.
///
class LayoutGameDefault: public LayoutGame
{
public:
	LayoutGameDefault(int width, int height);
	virtual ~LayoutGameDefault();

	void windowsInit();
	void windowsExit();

private:
	Window* leftmost;
	Window* middle_left;
	Window* middle_right;
	Window* rightmost;
	Window* next_container;

	std::vector<Window*> next;

	Window* leftmost_container;
	Window* score;
	Window* help_container;
	Window* help;
	Window* hscores_container;
	Window* hscores;
	Window* input_container;
	Window* input;
};

#endif //LAYOUTGAMEDEFAULT_H_DEFINED


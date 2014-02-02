#ifndef LAYOUTGAMEDEFAULT_H_DEFINED
#define LAYOUTGAMEDEFAULT_H_DEFINED

#include <Interface/Layout.hpp>
#include <Game/GameModeSurvival.hpp>
#include <vector>

/// Default game layout, suited for most  single-player
/// game modes.
///
class LayoutGameDefault: public Layout
{
public:
	LayoutGameDefault(GameModeSurvival* game, int width, int height);
	virtual ~LayoutGameDefault();

	void windowsInit();
	void windowsExit();

	void draw();

private:
	GameModeSurvival* game;

	Window* board;
	Window* info;
	Window* help;

	Window* leftmost;
	Window* middle_left;
	Window* middle_right;
	Window* rightmost;
	Window* next_container;

	std::vector<Window*> next;

	Window* leftmost_container;
	Window* score;
	Window* help_container;
	Window* hscores_container;
	Window* hscores;
	Window* hold;
};

#endif //LAYOUTGAMEDEFAULT_H_DEFINED


#ifndef ANIMATIONMAINMENU_H_DEFINED
#define ANIMATIONMAINMENU_H_DEFINED

#include <Interface/Window.hpp>
#include <Misc/Timer.hpp>

#include <string>
#include <vector>

/// A nice lil bubble to show going up.
struct Bubble
{
	int x;
	int y;
	Timer timer;
	int delay;
	std::string appearance;

	Bubble(int x, int y, int delay, std::string appearance);
	void update();
	void draw(Window* window);
};

///
class AnimationMainMenu
{
public:
	AnimationMainMenu(Window* window, int amount);
	virtual ~AnimationMainMenu();

	void update(Window* window);
	void draw(Window* window);

private:
	std::vector<Bubble*> bubbles;
	int bubblesThreshold;
};

#endif //ANIMATIONMAINMENU_H_DEFINED


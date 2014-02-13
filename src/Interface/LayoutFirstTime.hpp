#ifndef LAYOUTFIRSTTIME_H_DEFINED
#define LAYOUTFIRSTTIME_H_DEFINED

#include <Interface/Layout.hpp>
#include <Interface/Window.hpp>
#include <Interface/Menu.hpp>

/// How we show the screen at GameStateFirstTime.
/// See it's documentation there.
class LayoutFirstTime: public Layout
{
public:
	LayoutFirstTime(int width, int height);
	virtual ~LayoutFirstTime();

	void windowsInit();
	void windowsExit();

	void draw(Menu* menu);

	/// Where we'll query the profile name.
	Window* menu;
};

#endif //LAYOUTFIRSTTIME_H_DEFINED


#ifndef LAYOUTMAINMENU_H_DEFINED
#define LAYOUTMAINMENU_H_DEFINED

#include <Interface/Layout.hpp>
#include <Interface/Window.hpp>
#include <Interface/Menu.hpp>
#include <Interface/Animation/Animation.hpp>

/// How we show the screen at GameStateMainMenu.
/// See it's documentation there.
class LayoutMainMenu: public Layout
{
public:
	LayoutMainMenu(int width, int height);
	virtual ~LayoutMainMenu();

	void windowsInit();
	void windowsExit();

	/// Shows the Main Menu screen, along with drawing #menu.
	void draw(Menu* menu);

	Window* logo;
	Window* menu;

	// Sweet animation, beyb.
	Window* animationContainer;
	Animation* animation;
};

#endif //LAYOUTMAINMENU_H_DEFINED


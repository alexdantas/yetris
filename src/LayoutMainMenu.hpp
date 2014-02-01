#ifndef LAYOUTMAINMENU_H_DEFINED
#define LAYOUTMAINMENU_H_DEFINED

#include "Layout.hpp"
#include "Window.hpp"

///
class LayoutMainMenu
{
public:
	LayoutMainMenu(int width, int height);
	virtual ~LayoutMainMenu();

	void windowsInit();
	void windowsExit();

	Window* main;

	int width;
	int height;
	int originalWidth;
	int originalHeight;
};

#endif //LAYOUTMAINMENU_H_DEFINED


#ifndef LAYOUTMAINMENU_H_DEFINED
#define LAYOUTMAINMENU_H_DEFINED

#include <Interface/Layout.hpp>
#include <Interface/Window.hpp>

///
class LayoutMainMenu: public Layout
{
public:
	LayoutMainMenu(int width, int height);
	virtual ~LayoutMainMenu();

	void windowsInit();
	void windowsExit();

	void draw();
};

#endif //LAYOUTMAINMENU_H_DEFINED


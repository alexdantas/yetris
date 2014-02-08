#ifndef MENUITEMCHECKBOX_H_DEFINED
#define MENUITEMCHECKBOX_H_DEFINED

#include <Interface/MenuItem.hpp>

///
struct MenuItemCheckbox: public MenuItem
{
	MenuItemCheckbox(std::string label, int value);
	virtual ~MenuItemCheckbox() { };


	void draw(Window* window, int x, int y, int width, bool hilite=false);

	void handleInput(int input);

	void check(bool option);
	void toggle();
	bool isChecked();

	bool checked;
};

#endif //MENUITEMCHECKBOX_H_DEFINED


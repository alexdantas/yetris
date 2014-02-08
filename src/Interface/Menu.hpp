#ifndef MENU_H_DEFINED
#define MENU_H_DEFINED

#include <Interface/MenuItem.hpp>
#include <Interface/Window.hpp>

#include <vector>

class Menu
{
public:
	Menu(int x, int y, int width, int height);
	virtual ~Menu();

	void add(MenuItem* item);

	void draw(Window* window);

	/// Makes the menu react to #input, which is an Ncurses "char".
	void handleInput(int input);

	void goNext();
	void goPrevious();
	void goFirst();
	void goLast();

	/// Tells if the user selected an item that quits the menu.
	bool willQuit();

	/// Returns the user-specified value of the selected item.
	int getSelectedValue();

private:

	/// Container of all the options inside the menu.
	std::vector<MenuItem*> item;

	/// Index of the currently selected item.
	MenuItem* current;
	unsigned int currentIndex;

	MenuItem* first;
	MenuItem* last;

	int x;
	int y;
	int width;
	int height;

	bool selected;
	MenuItem* selectedItem;
};

#endif //MENU_H_DEFINED


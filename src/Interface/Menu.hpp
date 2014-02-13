#ifndef MENU_H_DEFINED
#define MENU_H_DEFINED

#include <Interface/Window.hpp>
#include <Interface/MenuItem.hpp>
#include <Interface/MenuItemCheckbox.hpp>
#include <Interface/MenuItemNumberbox.hpp>
#include <Interface/MenuItemTextbox.hpp>

#include <vector>

class Menu
{
public:
	Menu(int x, int y, int width, int height);
	virtual ~Menu();

	void add(MenuItem* item);
	void addBlank();

	void remove(int id);

	void draw(Window* window);

	/// Makes the menu react to #input, which is an Ncurses "char".
	void handleInput(int input);

	void goNext();
	void goPrevious();
	void goFirst();
	void goLast();

	/// Tells if the user selected an item that quits the menu.
	bool willQuit();

	/// Returns the user-specified id of the selected item.
	int currentID();

	/// Returns the bool internal value of item that has #id.
	/// @note Meaning the item must be a MenuItemCheckbox.
	bool getBool(int id);

	/// Returns the integer value of the item that has #id.
	/// @note Meaning the item must be a MenuItemNumberbox.
	int getInt(int id);

	/// Makes the menu able to be selected again.
	///
	/// When the user selects an item, #willQuit() returns
	/// true.
	/// By calling this function, you make possible to select
	/// things again.
	void reset();

	// FIXME: create an iterator so I dont need to expose
	//        this vector

	/// Container of all the options inside the menu.
	std::vector<MenuItem*> item;

private:

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


#include <Interface/MenuItemCheckbox.hpp>
#include <Config/Globals.hpp>

MenuItemCheckbox::MenuItemCheckbox(std::string label, int value, bool initial):
	MenuItem(label, value),
	checked(initial)
{
	this->type = MenuItem::CHECKBOX; // placing it above wont work
}

void MenuItemCheckbox::draw(Window* window, int x, int y, int width, bool hilite)
{
	int posx = x + width;

	MenuItem::draw(window, x, y, width, hilite);

	window->print("[", posx - 8, y, Globals::Theme::text);

	window->print("ON", posx - 7, y, ((this->checked) ?
	                                   Globals::Theme::hilite_text:
	                                   Globals::Theme::text));

	window->print("|", posx - 5, y, Globals::Theme::text);

	window->print("OFF", posx - 4, y, ((this->checked) ?
	                                    Globals::Theme::text :
	                                    Globals::Theme::hilite_text));

	window->print("]", posx - 1, y, Globals::Theme::text);
}
void MenuItemCheckbox::handleInput(int input)
{
	switch(input)
	{
	case KEY_LEFT:
		this->check(true);
		break;

	case KEY_RIGHT:
		this->check(false);
		break;

	case ' ':
	case '\n':
	case KEY_ENTER:
		this->toggle();
		break;
	}
}
void MenuItemCheckbox::check(bool option)
{
	this->checked = option;
}
void MenuItemCheckbox::toggle()
{
	this->checked = !(this->checked);
}
bool MenuItemCheckbox::isChecked()
{
	return this->checked;
}


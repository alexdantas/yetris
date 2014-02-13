#include <Interface/MenuItem.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

MenuItem::MenuItem(std::string label, int id):
	type(MenuItem::ITEM),
	label(label),
	id(id)
{ }

void MenuItem::draw(Window* window, int x, int y, int width, bool hilite)
{
	window->print(this->label.substr(0, width), x, y,
	              ((hilite) ?
	               Globals::Profiles::current->settings.theme.hilite_text:
	               Globals::Profiles::current->settings.theme.text));
}
void MenuItem::handleInput(int input)
{
	UNUSED(input);
}


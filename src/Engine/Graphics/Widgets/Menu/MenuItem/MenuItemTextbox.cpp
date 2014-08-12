#include <Interface/Menu/MenuItemTextbox.hpp>
#include <Config/Globals.hpp>
#include <Flow/InputManager.hpp>
#include <Misc/Utils.hpp>

MenuItemTextbox::MenuItemTextbox(std::string label, int id, int width, std::string initial):
	MenuItem(label, id),
	currentText(initial),
	width(width)
{

}
void MenuItemTextbox::draw(Window* window, int x, int y, int width, bool hilite)
{
	// Drawing label before actual textbox
	MenuItem::draw(window, x, y, width - (this->width) - 1, hilite);

	// Maximum x ids
	int posx = x + width;

	// Drawing the current text
	if ((int)(this->currentText.size()) > (this->width))
		window->print(this->currentText.substr(this->width), (posx - this->width), y, Globals::Profiles::current->settings.theme.textbox);
	else
		window->print(this->currentText, (posx - this->width), y, Globals::Profiles::current->settings.theme.textbox);

	int spacex = (posx - this->width) - this->currentText.size();

	// Drawing the rest of the spaces
	for (unsigned int i = 0; i < (this->width - this->currentText.size()); i++)
		window->printChar(' ', spacex + i, y, Globals::Profiles::current->settings.theme.textbox);
}

// Local function that tells in #input can be interpreted
// as a printable char (meaning no control character,
// as seen on the ASCII table).
bool isPrintable(int input)
{
	return ((input > 32) && (input < 127));
}

void MenuItemTextbox::handleInput()
{
	if (InputManager::noKeyPressed())
		return;

	if (isPrintable(InputManager::pressedKey))
	{
		this->currentText += (char)(InputManager::pressedKey);
		return;
	}

	if (InputManager::isPressed(KEY_BACKSPACE))
		Utils::String::pop_back(&this->currentText);
}


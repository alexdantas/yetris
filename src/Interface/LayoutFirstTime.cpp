#include <Interface/LayoutFirstTime.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

LayoutFirstTime::LayoutFirstTime(int width, int height):
	Layout(width, height),
	menu(NULL)
{
	this->windowsInit();
}
LayoutFirstTime::~LayoutFirstTime()
{
	this->windowsExit();
}
void LayoutFirstTime::windowsInit()
{
	this->menu = new Window(this->main,
	                        5,
	                        5,
	                        80,
	                        24);

	this->menu->setTitle("New Profile");
}
void LayoutFirstTime::windowsExit()
{
	clear();
	SAFE_DELETE(this->menu);
}
void LayoutFirstTime::draw(Menu* menu)
{
	UNUSED(menu);
	this->main->clear();

	this->main->print_multiline("Hello, there!\n"
	                            "It seems this is your first time running yetris.\n"
	                            "\n"
	                            "Please enter a profile name.\n"
	                            "It'll store all your settings, scores and game statistics.",
	                            1,
	                            1);

	this->main->print("Profile Name:", 1, 7, Colors::pair(COLOR_CYAN, COLOR_DEFAULT));
	this->main->print("(default: " +
	                  Utils::File::getUser() +
	                  ")",
	                  1,
	                  8,
	                  Colors::pair(COLOR_CYAN, COLOR_DEFAULT));

	this->main->print_multiline("It may not contain the following characters:\n"
	                            "\\ / . ^ ; # = ~",
	                            1,
	                            10,
	                            0);
	this->main->refresh();

	refresh();
}

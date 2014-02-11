#include <Interface/LayoutFirstTime.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

LayoutFirstTime::LayoutFirstTime(int width, int height):
	Layout(width, height)
{
	this->windowsInit();
}
LayoutFirstTime::~LayoutFirstTime()
{
	this->windowsExit();
}
void LayoutFirstTime::windowsInit()
{

}
void LayoutFirstTime::windowsExit()
{

}
void LayoutFirstTime::draw(Menu* menu)
{
	UNUSED(menu);
	this->main->clear();

	this->main->print("Press Enter to start game with default profile, " + Utils::File::getUser(), 0, 0, Colors::pair(COLOR_RED, COLOR_DEFAULT));

	this->main->refresh();
	refresh();
}


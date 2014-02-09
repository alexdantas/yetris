#include <Interface/LayoutMainMenu.hpp>
#include <Interface/Colors.hpp>
#include <Interface/Ncurses.hpp>
#include <Interface/Animation/AnimationFire.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

#include <iostream>

LayoutMainMenu::LayoutMainMenu(int width, int height):
	Layout(width, height),
	logo(nullptr),
	menu(nullptr),
	animationContainer(nullptr),
	animation(nullptr)
{
	this->windowsInit();
}
LayoutMainMenu::~LayoutMainMenu()
{
	this->windowsExit();
}
void LayoutMainMenu::windowsInit()
{
	// LOGO
	this->logo = new Window(this->main,
	                        0,
	                        0,
	                        0,
	                        7);

	if (Globals::Screen::show_borders)
	{
		this->logo->borders(Globals::Screen::fancy_borders ?
		                    Window::BORDER_FANCY :
		                    Window::BORDER_REGULAR);
	}
	this->logo->refresh();

	// MENU
	this->menu = new Window(this->main,
	                        this->main->getW() / 3,
	                        10,
	                        this->main->getW() / 3,
	                        12);

	if (Globals::Screen::show_borders)
	{
		this->menu->borders(Globals::Screen::fancy_borders ?
		                    Window::BORDER_FANCY :
		                    Window::BORDER_REGULAR);
	}
	this->menu->refresh();

	// Just need to create the animation below the logo
	int height = this->main->getH() - this->logo->getH() - 1;
	int posy   = this->main->getH() - height - 1;

	this->animationContainer = new Window(this->main,
	                                      0,
	                                      posy,
	                                      0,
	                                      height);

	// DECIDE HERE THE TYPE OF THE ANIMATION
	this->animation = new AnimationFire(this->animationContainer);
	this->animation->load();
}
void LayoutMainMenu::windowsExit()
{
	SAFE_DELETE(this->menu);
	SAFE_DELETE(this->logo);
	SAFE_DELETE(this->animationContainer);
	SAFE_DELETE(this->animation);
}
void LayoutMainMenu::draw(Menu* menu)
{
	this->main->clear();

	this->animationContainer->clear();

	this->animation->update();
	this->animation->draw();

	this->animationContainer->refresh();

	this->logo->clear();
	this->logo->print_multiline("             __        __       \n"
	                            ".--.--.-----|  |_.----|__.-----.\n"
	                            "|  |  |  -__|   _|   _|  |__ --|\n"
	                            "|___  |_____|____|__| |__|_____|\n"
	                            "|_____|                         ",
	                            this->logo->getW() / 2 - 32/2,
	                            1,
	                            Colors::pair(COLOR_RED, COLOR_DEFAULT));
	this->logo->refresh();

	// Yay!
	this->menu->clear();
	menu->draw(this->menu);
	this->menu->refresh();

	this->main->refresh();

	// NCURSES NEEDS THIS
	refresh();
}


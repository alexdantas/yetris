#include <Game/Display/Layouts/LayoutMainMenu.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/Graphics/Animation/AnimationFire.hpp>
#include <Engine/Graphics/Animation/AnimationWater.hpp>
#include <Engine/Graphics/Animation/AnimationSnakes.hpp>
#include <Engine/Graphics/Animation/AnimationGameOfLife.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/String.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Game/Config/Globals.hpp>
#include <Game/States/GameStateMainMenu.hpp>
#include <Game/Entities/Profile.hpp>

#include <iostream>

LayoutMainMenu::LayoutMainMenu(int width, int height, GameStateMainMenu* state):
	Layout(width, height),
	state(state),
	logo(NULL),
	menu(NULL),
	animationContainer(NULL),
	animation(NULL)
{
	this->windowsInit();
}
LayoutMainMenu::~LayoutMainMenu()
{
	this->windowsExit();
}
void LayoutMainMenu::windowsInit()
{
	Layout::windowsInit();

	// LOGO
	this->logo = new Window(this->main,
	                        0,
	                        0,
	                        0,
	                        9);

	if (EngineGlobals::Screen::show_borders)
	{
		this->logo->borders(EngineGlobals::Screen::fancy_borders ?
		                    Window::BORDER_FANCY :
		                    Window::BORDER_REGULAR);
	}
	// Profile name with an "'s" appended
	// (like "Rachel's" or "Chris'")
	std::string name = Globals::Profiles::current->name;
	if (Utils::String::back(name) == 's')
		name += '\'';
	else
		name += "'s";

	this->logo->setTitle(name);
	this->logo->clear();
	this->logo->refresh();

	// Menu Window (that little thing in the middle)
	this->menu = new Window(this->main,
	                        (this->main->getW() / 3) - 2,
	                        this->logo->getH() + 1,
	                        (this->main->getW() / 3) + 2,
	                        this->main->getH() - this->logo->getH() - 2);

	// Just need to create the animation below the logo
	int height = this->main->getH() - this->logo->getH() - 1;
	int posy   = this->main->getH() - height - 1;

	this->animationContainer = new Window(this->main,
	                                      0, posy,
	                                      0, height);
	this->animationContainer->borders(Window::BORDER_NONE);

	if (Globals::Profiles::current->settings.screen.animation_menu == "random")
	{
		// Deciding randomly the type of the Animation
		switch(Utils::Random::between(0, 3))
		{
		case 0:
			this->animation = new AnimationWater(this->animationContainer);
			break;

		case 1:
			this->animation = new AnimationSnakes(this->animationContainer);
			break;

		case 2:
			this->animation = new AnimationGameOfLife(this->animationContainer);
			break;

		default:
			this->animation = new AnimationFire(this->animationContainer);
			break;
		}
	}
	else if (Globals::Profiles::current->settings.screen.animation_menu == "fire")
		this->animation = new AnimationFire(this->animationContainer);

	else if (Globals::Profiles::current->settings.screen.animation_menu == "water")
		this->animation = new AnimationWater(this->animationContainer);

	else if (Globals::Profiles::current->settings.screen.animation_menu == "snakes")
		this->animation = new AnimationSnakes(this->animationContainer);

	else if (Globals::Profiles::current->settings.screen.animation_menu == "life")
		this->animation = new AnimationGameOfLife(this->animationContainer);

	else
		this->animation = NULL;

	if (this->animation)
		this->animation->load();
}
void LayoutMainMenu::windowsExit()
{
	SAFE_DELETE(this->menu);
	SAFE_DELETE(this->logo);
	SAFE_DELETE(this->animationContainer);
	SAFE_DELETE(this->animation);

	Layout::windowsExit();
}
void LayoutMainMenu::draw(Menu* menu)
{
	this->main->clear();

	this->animationContainer->clear();

	if (this->animation)
	{
		this->animation->update();
		this->animation->draw();
	}

	this->animationContainer->refresh();

	this->logo->clear();
	this->logo->print(Utils::String::split(" __ __    ___ ______  ____   ____ _____\n"
	                                       "|  |  |  /  _]      ||    \\ |    / ___/\n"
	                                       "|  |  | /  [_|      ||  D  ) |  (   \\_\n"
	                                       "|  ~  ||    _]_|  |_||    /  |  |\\__  |\n"
	                                       "|___, ||   [_  |  |  |    \\  |  |/  \\ |\n"
	                                       "|     ||     | |  |  |  .  \\ |  |\\    |\n"
	                                       "|____/ |_____| |__|  |__|\\_||____|\\___|", '\n'),
	                  this->logo->getW() / 2 - 39/2 - 1,
	                  1);

	this->logo->refresh();

	// Yay!
	this->menu->clear();

	if (this->state->menuProfilesActivated)
	{
		this->menu->print("C", 1, 1, Globals::Profiles::current->settings.theme.hilite_text);
		this->menu->print("reate new Profile", 2, 1, Globals::Profiles::current->settings.theme.text);

		this->menu->print("D", 1, 2, Globals::Profiles::current->settings.theme.hilite_text);
		this->menu->print("elete Profile", 2, 2, Globals::Profiles::current->settings.theme.text);

		this->menu->print("S", 1, 3, Globals::Profiles::current->settings.theme.hilite_text);
		this->menu->print("witch to Profile", 2, 3, Globals::Profiles::current->settings.theme.text);

	}
	menu->draw(this->menu);

	this->menu->refresh();

	this->main->refresh();

	// NCURSES NEEDS THIS
	refresh();
}


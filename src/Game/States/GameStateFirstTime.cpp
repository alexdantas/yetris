#include <Game/States/GameStateFirstTime.hpp>
#include <Game/States/GameStateMainMenu.hpp>
#include <Game/Entities/Profile.hpp>
#include <Engine/Flow/StateManager.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/File.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Game/Config/Globals.hpp>

#include <cstring>

GameStateFirstTime::GameStateFirstTime():
	layout(NULL),
	name("")
{ }
GameStateFirstTime::~GameStateFirstTime()
{ }
void GameStateFirstTime::load()
{
	this->layout = new LayoutFirstTime(80, 24);
}

void GameStateFirstTime::unload()
{
	SAFE_DELETE(this->layout);
}

void GameStateFirstTime::update()
{
	// User typed already
	if (! this->name.empty())
	{
		Globals::Profiles::current = new Profile(this->name);
		StateManager::change(new GameStateMainMenu());
	}
}

void GameStateFirstTime::draw()
{
	// BIG HACK
	//
	// Instead of drawing, we actually query the user for
	// a profile name
	//
	// NEED to remove this
	// Are you kidding me?
	// Direct ncurses calls?

	clear();

	mvaddstr(0, 0,
	         "Hello, there!\n"
	         "It seems this is your first time running yetris.\n"
	         "\n"
	         "Please enter a profile name.\n"
	         "It'll store all your settings, scores and game statistics.");

	attron(A_REVERSE);
	mvaddstr(7, 0, "Profile Name:");
	attroff(A_REVERSE);

	mvaddstr(8, 0,  "default:");
	mvaddstr(8, 9, Utils::File::getUser().c_str());

	mvaddstr(10, 0,
	         "It may not contain the following characters:\n"
	         "\\ / . ^ ; # = ~");

	refresh();

	// Making everything "right"
	nocbreak();
	curs_set(1);
	echo();
	nodelay(stdscr, FALSE);

	char name[256];
	std::memset(name, '\0', 256);

	mvgetnstr(7, 15, name, 255);

	if (Profile::isNameValid(name))
	{
		if (strlen(name) == 0)
			this->name = Utils::File::getUser();
		else
			this->name = name;
	}

	// Returning to ncurses' "wrong" mode
	nodelay(stdscr, TRUE);
	noecho();
	curs_set(0);
	cbreak();
}


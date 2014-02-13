#include <Flow/GameStateFirstTime.hpp>
#include <Misc/Utils.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>

#include <cstring>

GameStateFirstTime::GameStateFirstTime():
	layout(NULL),
	name("")
{ }
GameStateFirstTime::~GameStateFirstTime()
{ }
void GameStateFirstTime::load(int stack)
{
	UNUSED(stack);

	this->layout = new LayoutFirstTime(80, 24);
}

int GameStateFirstTime::unload()
{
	SAFE_DELETE(this->layout);

	return 0;
}

GameState::StateCode GameStateFirstTime::update()
{
	// User typed already
	if (! this->name.empty())
	{
		Globals::Profiles::current = new Profile(this->name);
		return GameState::MAIN_MENU;
	}

	return GameState::CONTINUE;
}

bool isNameValid(std::string name)
{
	// Not allowed characters
	if ((name.find('/')  != std::string::npos) ||
	    (name.find('\\') != std::string::npos) ||
	    (name.find(';')  != std::string::npos) ||
	    (name.find('#')  != std::string::npos) ||
	    (name.find('=')  != std::string::npos) ||
	    (name.find('.')  != std::string::npos))
		return false;

	return true;
}

void GameStateFirstTime::draw()
{
	this->layout->draw(NULL);

	// BIG HACK
	//
	// Instead of drawing, we actually query the user for
	// a profile name
	//
	// NEED to remove this
	// Are you kidding me?
	// Direct ncurses calls?

	// Making everything "right"
	nocbreak();
	curs_set(1);
	echo();
	nodelay(stdscr, FALSE);

	char name[256];
	std::memset(name, '\0', 256);

	mvgetnstr(7, 15, name, 255);

	if (isNameValid(name))
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


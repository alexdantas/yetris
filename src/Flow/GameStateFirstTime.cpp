#include <Flow/GameStateFirstTime.hpp>
#include <Misc/Utils.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>

GameStateFirstTime::GameStateFirstTime():
	layout(NULL),
	menu(NULL)
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
	SAFE_DELETE(this->menu);

	return 0;
}

GameState::StateCode GameStateFirstTime::update()
{
	int input = Ncurses::getInput(100);

	if (input == '\n' || input == KEY_ENTER)
	{
		Globals::Profiles::current = new Profile(Utils::File::getUser() + "LEL");

		return GameState::MAIN_MENU;
	}

	return GameState::CONTINUE;
}

void GameStateFirstTime::draw()
{
	this->layout->draw(this->menu);
}


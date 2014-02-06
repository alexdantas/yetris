#include <Flow/GameStateMainMenu.hpp>
#include <Interface/Ncurses.hpp>
#include <Misc/Utils.hpp>

GameStateMainMenu::GameStateMainMenu():
	layout(NULL),
	gameMode(0)
{ }
GameStateMainMenu::~GameStateMainMenu()
{

}
void GameStateMainMenu::load(int stack)
{
	UNUSED(stack);

	this->layout = new LayoutMainMenu(80, 24);
}

int GameStateMainMenu::unload()
{
	SAFE_DELETE(this->layout);

	return this->gameMode;
}

GameState::StateCode GameStateMainMenu::update()
{
	switch(Ncurses::getInput(100))
	{
	case 'q':
		return GameState::QUIT;

	case '1':
		this->gameMode = 1;
		return GameState::GAME_START;

	case '2':
		this->gameMode = 2;
		return GameState::GAME_START;
	}

	return GameState::CONTINUE;
}

void GameStateMainMenu::draw()
{
	this->layout->draw();
}



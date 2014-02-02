#include <Flow/GameStateGame.hpp>
#include <Flow/StateManager.hpp>
#include <Game/GameModeSurvival.hpp>
#include <Interface/Ncurses.hpp>
#include <Interface/LayoutGameDefault.hpp>
#include <Misc/Utils.hpp>

GameStateGame::GameStateGame():
	layout(nullptr),
	game(nullptr),
	willQuit(false)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
	(void)(stack);

	this->game = new GameModeSurvival();
	this->layout = new LayoutGameDefault((GameModeSurvival*)game, 80, 24);

	this->game->start();
}
int GameStateGame::unload()
{
	SAFE_DELETE(this->game);
	SAFE_DELETE(this->layout);

	return 0;
}
GameState::StateCode GameStateGame::update()
{
	if (this->willQuit)
		return GameState::QUIT;

	this->game->handleInput(Ncurses::getInput(100));
	this->game->update();

	if (this->game->isOver())
	{
		Ncurses::delay_ms(500);
		this->game->start();
	}

	if (this->game->willQuit())
		this->willQuit = true;

	return GameState::CONTINUE;
}
void GameStateGame::draw()
{
	this->layout->draw();
}


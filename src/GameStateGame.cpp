#include "GameStateGame.hpp"
#include "StateManager.hpp"
#include "Ncurses.hpp"
#include "LayoutGameDefault.hpp"
#include "GameModeSurvival.hpp"

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

	this->layout = new LayoutGameDefault(80, 24);

	this->game = new GameModeSurvival(this->layout);
	this->game->start();
}
int GameStateGame::unload()
{
	delete this->game;
	delete this->layout;

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
	this->game->draw();
}


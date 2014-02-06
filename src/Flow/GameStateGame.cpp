#include <Flow/GameStateGame.hpp>
#include <Flow/StateManager.hpp>
#include <Game/GameModeSurvival.hpp>
#include <Misc/Utils.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>

GameStateGame::GameStateGame():
	game(nullptr),
	willQuit(false)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int gameMode)
{
	switch(gameMode)
	{
	case 2:
		Globals::Game::invisible = true;
		break;

	case 3:
		Globals::Game::slide_left = true;
		break;

	case 4:
		Globals::Game::slide_right = true;
		break;

	default:
		break;
	}

	this->game = new GameModeSurvival();
	this->game->start();
}
int GameStateGame::unload()
{
	SAFE_DELETE(this->game);

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


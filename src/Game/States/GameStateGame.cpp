#include <Game/States/GameStateGame.hpp>
#include <Engine/Flow/StateManager.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Game/Config/Globals.hpp>
#include <Game/Entities/Profile.hpp>

GameStateGame::GameStateGame():
	game(NULL),
	willQuit(false)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
	UNUSED(stack);
	SAFE_DELETE(this->game);

	try {
		Globals::Profiles::current->scores->load();
	}
	catch (ScoreFileException)
	{
		// File doesn't exist
		// ...carry on
	}

	this->game = new Game();
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

	this->game->handleInput();
	this->game->update();

	if (this->game->isOver())
	{
		Globals::Profiles::current->scores->handle(&Globals::Profiles::current->scores->score);
		Globals::Profiles::current->scores->save();

		Ncurses::delay_ms(500);

		// Restart the game
		this->load();
//		this->game->start();
	}

	if (this->game->willQuit())
		this->willQuit = true;

	if (this->game->willReturnToMenu())
		return GameState::MAIN_MENU;

	return GameState::CONTINUE;
}
void GameStateGame::draw()
{
	this->game->draw();
}


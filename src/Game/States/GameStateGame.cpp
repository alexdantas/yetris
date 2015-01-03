#include <Game/States/GameStateGame.hpp>
#include <Game/States/GameStateMainMenu.hpp>
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
void GameStateGame::load()
{
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
void GameStateGame::unload()
{
	SAFE_DELETE(this->game);
}
void GameStateGame::update()
{
	if (this->willQuit)
		StateManager::quit();

	this->game->handleInput();
	this->game->update();

	if (this->game->isOver())
	{
		Globals::Profiles::current->scores->handle(&Globals::Profiles::current->scores->score);
		Globals::Profiles::current->scores->save();

		Utils::Time::delay_ms(500);

		// Restart the game
		this->load();
	}

	if (this->game->willQuit())
		this->willQuit = true;

	if (this->game->willReturnToMenu())
		StateManager::change(new GameStateMainMenu());
}
void GameStateGame::draw()
{
	this->game->draw();
}


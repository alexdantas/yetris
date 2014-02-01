#include "StateManager.hpp"
#include "GameStateGame.hpp"
#include "GameStateMainMenu.hpp"

StateManager::StateManager():
	currentState(nullptr),
	sharedInfo(0)
{
	// The first state, Hardcoded
	this->currentState = new GameStateMainMenu();

	this->currentState->load();
	this->sharedInfo = 0;
}
StateManager::~StateManager()
{
	if (this->currentState)
	{
		this->currentState->unload();
		delete this->currentState;

		this->currentState = nullptr; // you never know
	}
}
void StateManager::run()
{
	bool letsQuit = false;

	while (!letsQuit)
	{
		// Updating the whole state.
		// This value is returned from it tell us if
		// we need to switch from the current state.
		GameState::StateCode whatToDoNow;

		whatToDoNow = this->currentState->update();

		switch (whatToDoNow)
		{
		case GameState::CONTINUE:
			// Just continue on the current state.
			break;

		case GameState::QUIT:
			letsQuit = true;
			break;

		case GameState::GAME_START:
			this->currentState->unload();
			delete this->currentState;

			this->currentState = new GameStateGame();
			this->currentState->load();
			break;

		default:
			break;
		}

		this->currentState->draw();
	}
}


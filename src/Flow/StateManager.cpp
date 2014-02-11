#include <Flow/StateManager.hpp>
#include <Flow/GameStateGame.hpp>
#include <Flow/GameStateMainMenu.hpp>
#include <Misc/Utils.hpp>
#include <Game/Profile.hpp>
#include <Config/Globals.hpp>

// Local function that assures #name is loaded.
void loadProfile(std::string name)
{
	SAFE_DELETE(Globals::profile); // you never know

	Globals::profile = new Profile(name);
}

StateManager::StateManager():
	currentState(nullptr),
	sharedInfo(0)
{
	// First we'll load the default profile.
	std::string name = Profile::load();
	if (! name.empty())
		loadProfile(name);
	else
		loadProfile("default");

	// The first state, Hardcoded
	this->currentState = new GameStateMainMenu();
	this->currentState->load();
}
StateManager::~StateManager()
{
	if (this->currentState)
		this->currentState->unload();

	SAFE_DELETE(this->currentState);
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
		{
			this->currentState->unload();
			delete this->currentState;

			this->currentState = new GameStateGame();
			this->currentState->load();
			break;
		}

		case GameState::MAIN_MENU:
		{
			this->currentState->unload();
			delete this->currentState;

			this->currentState = new GameStateMainMenu();
			this->currentState->load();
			break;
		}

		default:
			break;
		}

		this->currentState->draw();
	}
}


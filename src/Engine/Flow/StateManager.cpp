#include <Engine/Flow/StateManager.hpp>
#include <Game/States/GameStateGame.hpp>
#include <Game/States/GameStateMainMenu.hpp>
#include <Game/States/GameStateFirstTime.hpp>
#include <Engine/InputManager.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Game/Entities/Profile.hpp>
#include <Game/Config/Globals.hpp>
#include <Engine/Helpers/INI.hpp>

StateManager::StateManager():
	currentState(NULL),
	sharedInfo(0)
{
	// First we'll load the default profile.
	if (! Profile::load())
	{
		// Couldn't find any profiles - first time!

		Globals::Profiles::default_name = "";

		// Let's ask for the user
		this->currentState = new GameStateFirstTime();
		this->currentState->load();
	}
	else
	{
		// Alright, let's load it!

		// this is initialized on Config::init - if the
		// global config file is found
		if (Globals::Profiles::default_name.empty())
			Globals::Profiles::default_name = Profile::profiles.front();

		Globals::Profiles::current = new Profile(Globals::Profiles::default_name);
		Globals::Profiles::current->loadSettings();

		// The first state, Hardcoded
		this->currentState = new GameStateMainMenu();
		this->currentState->load();
	}
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
		InputManager::update();

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
			this->currentState->unload();
			delete this->currentState;
			this->currentState = NULL;

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

		if (this->currentState)
			this->currentState->draw();
	}

	// Right before quitting, we must save current
	// user's settings
	Globals::Profiles::current->saveSettings();

	// And set the current profile as the default
	// to load next time.
	INI::Parser* ini;

	try
	{
		ini = new INI::Parser(Globals::Config::file);
	}
	catch(std::runtime_error& e)
	{
		// File doesn't exist!
		// Silently create
		ini = new INI::Parser();
		ini->create();
	}

	ini->top().addGroup("profiles");
	(*ini)("profiles").addKey("default", Globals::Profiles::current->name);

	try
	{
		ini->saveAs(Globals::Config::file);
	}
	catch(std::runtime_error& e)
	{
		// Couldn't save the file...
		// ...do nothing
	}
	SAFE_DELETE(ini);
}


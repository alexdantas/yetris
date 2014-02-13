#include <Flow/GameStateMainMenu.hpp>
#include <Interface/Ncurses.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	// Main Menu
	SINGLE_PLAYER,
	PROFILES,
	QUIT_GAME,

	// Single Player Submenu
	START_GAME,
	GO_BACK,
	STARTING_LEVEL,
	INITIAL_NOISE,
	INVISIBLE,
	SLIDE_LEFT,
	SLIDE_RIGHT,
	HOLD_PIECE,
	GHOST_PIECE,
	SHOW_STATISTICS
};

GameStateMainMenu::GameStateMainMenu():
	layout(NULL),
	menu(NULL),
	menuSinglePlayer(NULL),
	menuSinglePlayerActivated(false)
{ }
GameStateMainMenu::~GameStateMainMenu()
{ }
void GameStateMainMenu::load(int stack)
{
	UNUSED(stack);

	this->layout = new LayoutMainMenu(80, 24);

	// Creating the Menu and Items.
	// Their default values will be based on current Profile's
	// settings.
	this->menu = new Menu(1,
	                      1,
	                      this->layout->menu->getW() - 2,
	                      this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Single Player", SINGLE_PLAYER);
	menu->add(item);

	item = new MenuItem("Profiles", PROFILES);
	menu->add(item);

	item = new MenuItem("Quit", QUIT_GAME);
	menu->add(item);

	this->menuSinglePlayer = new Menu(1,
	                                  1,
	                                  this->layout->menu->getW() - 2,
	                                  this->layout->menu->getH() - 2);


	item = new MenuItem("Start Game", START_GAME);
	menuSinglePlayer->add(item);

	item = new MenuItem("Back", GO_BACK);
	menuSinglePlayer->add(item);

	menuSinglePlayer->addBlank();

	MenuItemNumberbox* number;

	number = new MenuItemNumberbox("Starting Level", STARTING_LEVEL, 1, 18, Globals::Profiles::current->settings.game.starting_level);
	menuSinglePlayer->add(number);

	number = new MenuItemNumberbox("Initial Noise", INITIAL_NOISE, 0, 20, Globals::Profiles::current->settings.game.initial_noise);
	menuSinglePlayer->add(number);

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Invisible",
	                             INVISIBLE,
	                             Globals::Profiles::current->settings.game.invisible);
	menuSinglePlayer->add(check);

	check = new MenuItemCheckbox("Slide Left",
	                             SLIDE_LEFT,
	                             Globals::Profiles::current->settings.game.slide_left);
	menuSinglePlayer->add(check);

	check = new MenuItemCheckbox("Slide Right",
	                             SLIDE_RIGHT,
	                             Globals::Profiles::current->settings.game.slide_right);
	menuSinglePlayer->add(check);

	check = new MenuItemCheckbox("Hold Piece",
	                             HOLD_PIECE,
	                             Globals::Profiles::current->settings.game.can_hold);
	menuSinglePlayer->add(check);

	check = new MenuItemCheckbox("Ghost Piece",
	                             GHOST_PIECE,
	                             Globals::Profiles::current->settings.game.has_ghost);
	menuSinglePlayer->add(check);

	check = new MenuItemCheckbox("Show Statistics",
	                             SHOW_STATISTICS,
	                             Globals::Profiles::current->settings.screen.show_statistics);
	menuSinglePlayer->add(check);
}

int GameStateMainMenu::unload()
{
	// Copying all options on the Menu to the global settings
	for (unsigned int i = 0; i < (menuSinglePlayer->item.size()); i++)
	{
		if (! menuSinglePlayer->item[i])
			continue;

		// TODO: Learn a better way to cast from parent classes
		//       to child classes.
		// WARNING: Make sure the types are correct otherwise will
		//          screw up with NULL things.

		MenuItemNumberbox* num   = NULL;
		MenuItemCheckbox*  check = NULL;

		if (menuSinglePlayer->item[i]->type == MenuItem::NUMBERBOX)
			num = (MenuItemNumberbox*)menuSinglePlayer->item[i];

		else if (menuSinglePlayer->item[i]->type == MenuItem::CHECKBOX)
			check = (MenuItemCheckbox*)menuSinglePlayer->item[i];

		switch(menuSinglePlayer->item[i]->value)
		{
		case STARTING_LEVEL:
			if (num)
				Globals::Profiles::current->settings.game.starting_level = num->current;
			break;

		case INITIAL_NOISE:
			if (num)
				Globals::Profiles::current->settings.game.initial_noise = num->current;
			break;

		case INVISIBLE:
			if (check)
				Globals::Profiles::current->settings.game.invisible = check->isChecked();
			break;

		case SLIDE_LEFT:
			if (check)
				Globals::Profiles::current->settings.game.slide_left = check->isChecked();
			break;

		case SLIDE_RIGHT:
			if (check)
				Globals::Profiles::current->settings.game.slide_right = check->isChecked();
			break;

		case HOLD_PIECE:
			if (check)
				Globals::Profiles::current->settings.game.can_hold = check->isChecked();
			break;

		case GHOST_PIECE:
			if (check)
				Globals::Profiles::current->settings.game.has_ghost = check->isChecked();
			break;

		case SHOW_STATISTICS:
			if (check)
				Globals::Profiles::current->settings.screen.show_statistics = check->isChecked();
			break;
		}
	}

	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->menuSinglePlayer);
	SAFE_DELETE(this->menu);

	return 0;
}

GameState::StateCode GameStateMainMenu::update()
{
	int input = Ncurses::getInput(100);

	if (input == 'q')
		return GameState::QUIT;

	if (this->menuSinglePlayerActivated)
	{
		this->menuSinglePlayer->handleInput(input);

		if (this->menuSinglePlayer->willQuit())
		{
			// User selected an option
			// Let's get values from menu items
			Globals::Profiles::current->settings.game.initial_noise  = this->menuSinglePlayer->getInt(INITIAL_NOISE);
			Globals::Profiles::current->settings.game.starting_level = (unsigned int)this->menuSinglePlayer->getInt(STARTING_LEVEL);

			Globals::Profiles::current->settings.game.invisible   = this->menuSinglePlayer->getBool(INVISIBLE);
			Globals::Profiles::current->settings.game.slide_left  = this->menuSinglePlayer->getBool(SLIDE_LEFT);
			Globals::Profiles::current->settings.game.slide_right = this->menuSinglePlayer->getBool(SLIDE_RIGHT);
			Globals::Profiles::current->settings.game.can_hold    = this->menuSinglePlayer->getBool(HOLD_PIECE);
			Globals::Profiles::current->settings.game.can_hold    = this->menuSinglePlayer->getBool(HOLD_PIECE);
			Globals::Profiles::current->settings.game.has_ghost   = this->menuSinglePlayer->getBool(GHOST_PIECE);
			Globals::Profiles::current->settings.screen.show_statistics = this->menuSinglePlayer->getBool(SHOW_STATISTICS);

			// And then exit based on the selected option.
			switch (this->menuSinglePlayer->getSelectedValue())
			{
			case START_GAME:
				return GameState::GAME_START;
				break;

			case GO_BACK:
				this->menuSinglePlayerActivated = false;
				break;
			}
			this->menuSinglePlayer->reset();
		}
	}
	else
	{
		// We're still at the Main Menu
		this->menu->handleInput(input);

		if (this->menu->willQuit())
		{
			switch(this->menu->getSelectedValue())
			{
			case SINGLE_PLAYER:
				this->menuSinglePlayerActivated = true;
				break;

			case PROFILES:
				// yeah
				break;

			case QUIT_GAME:
				return GameState::QUIT;
				break;
			}
			this->menu->reset();
		}
	}

	// Otherwise, continuing things...
	return GameState::CONTINUE;
}

void GameStateMainMenu::draw()
{
	if (this->menuSinglePlayerActivated)
		this->layout->draw(this->menuSinglePlayer);

	else
		this->layout->draw(this->menu);
}



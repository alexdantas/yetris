#include <Flow/GameStateMainMenu.hpp>
#include <Interface/Ncurses.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	START_GAME,
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
	menu(NULL)
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

	item = new MenuItem("Start Game", START_GAME);
	menu->add(item);

	menu->addBlank();

	MenuItemNumberbox* number;

	number = new MenuItemNumberbox("Starting Level", STARTING_LEVEL, 1, 18, Globals::Profiles::current->settings.game.starting_level);
	menu->add(number);

	number = new MenuItemNumberbox("Initial Noise", INITIAL_NOISE, 0, 20, Globals::Profiles::current->settings.game.initial_noise);
	menu->add(number);

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Invisible",
	                             INVISIBLE,
	                             Globals::Profiles::current->settings.game.invisible);
	menu->add(check);

	check = new MenuItemCheckbox("Slide Left",
	                             SLIDE_LEFT,
	                             Globals::Profiles::current->settings.game.slide_left);
	menu->add(check);

	check = new MenuItemCheckbox("Slide Right",
	                             SLIDE_RIGHT,
	                             Globals::Profiles::current->settings.game.slide_right);
	menu->add(check);

	check = new MenuItemCheckbox("Hold Piece",
	                             HOLD_PIECE,
	                             Globals::Profiles::current->settings.game.can_hold);
	menu->add(check);

	check = new MenuItemCheckbox("Ghost Piece",
	                             GHOST_PIECE,
	                             Globals::Profiles::current->settings.game.has_ghost);
	menu->add(check);

	check = new MenuItemCheckbox("Show Statistics",
	                             SHOW_STATISTICS,
	                             Globals::Profiles::current->settings.screen.show_statistics);
	menu->add(check);
}

int GameStateMainMenu::unload()
{
	// Copying all options on the Menu to the global settings
	for (unsigned int i = 0; i < (menu->item.size()); i++)
	{
		if (! menu->item[i])
			continue;

		// TODO: Learn a better way to cast from parent classes
		//       to child classes.
		// WARNING: Make sure the types are correct otherwise will
		//          screw up with NULL things.

		MenuItemNumberbox* num   = NULL;
		MenuItemCheckbox*  check = NULL;

		if (menu->item[i]->type == MenuItem::NUMBERBOX)
			num = (MenuItemNumberbox*)menu->item[i];

		else if (menu->item[i]->type == MenuItem::CHECKBOX)
			check = (MenuItemCheckbox*)menu->item[i];

		switch(menu->item[i]->value)
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
	SAFE_DELETE(this->menu);

	return 0;
}

GameState::StateCode GameStateMainMenu::update()
{
	int input = Ncurses::getInput(100);

	if (input == 'q')
		return GameState::QUIT;

	this->menu->handleInput(input);

	if (this->menu->willQuit())
	{
		// User selected an option
		// Let's get values from menu items
		Globals::Profiles::current->settings.game.initial_noise  = this->menu->getInt(INITIAL_NOISE);
		Globals::Profiles::current->settings.game.starting_level = (unsigned int)this->menu->getInt(STARTING_LEVEL);

		Globals::Profiles::current->settings.game.invisible   = this->menu->getBool(INVISIBLE);
		Globals::Profiles::current->settings.game.slide_left  = this->menu->getBool(SLIDE_LEFT);
		Globals::Profiles::current->settings.game.slide_right = this->menu->getBool(SLIDE_RIGHT);
		Globals::Profiles::current->settings.game.can_hold    = this->menu->getBool(HOLD_PIECE);
		Globals::Profiles::current->settings.game.can_hold    = this->menu->getBool(HOLD_PIECE);
		Globals::Profiles::current->settings.game.has_ghost   = this->menu->getBool(GHOST_PIECE);
		Globals::Profiles::current->settings.screen.show_statistics = this->menu->getBool(SHOW_STATISTICS);

		// And then exit based on the selected option.
		if (this->menu->getSelectedValue() == START_GAME)
			return GameState::GAME_START;
	}

	return GameState::CONTINUE;
}

void GameStateMainMenu::draw()
{
	this->layout->draw(this->menu);
}



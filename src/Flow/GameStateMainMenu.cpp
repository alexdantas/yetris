#include <Flow/GameStateMainMenu.hpp>
#include <Flow/InputManager.hpp>
#include <Interface/Ncurses.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	// Main Menu
	SINGLE_PLAYER,
	OPTIONS,
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

	// Options Submenu
	SHOW_BORDERS,
	FANCY_BORDERS,
	OUTER_BORDER,
	USE_COLORS,
	CENTER_HORIZONTAL,
	CENTER_VERTICAL,
	SHOW_STATISTICS,
	RANDOM_ALGORITHM,
	LINE_DELAY,
	NEXT_PIECES,

	// Profiles Submenu
	PROFILES_NAME
};

// I need this so when we add/remove/change profiles
// from the menu things don't get messed up.
unsigned int profileMenuIndex = 0;

GameStateMainMenu::GameStateMainMenu():
	layout(nullptr),
	menu(nullptr),
	menuSinglePlayer(nullptr),
	menuSinglePlayerActivated(false),
	menuOptions(nullptr),
	menuOptionsActvated(false),
	menuProfiles(nullptr),
	menuProfilesActivated(false)
{ }
GameStateMainMenu::~GameStateMainMenu()
{ }
void GameStateMainMenu::load(int stack)
{
	UNUSED(stack);

	this->layout = new LayoutMainMenu(80, 24, this);

	createMainMenu();
	createSinglePlayerMenu();
	createOptionsMenu();
	createProfilesMenu();
}

int GameStateMainMenu::unload()
{
	saveSettingsMenuSinglePlayer();
	saveSettingsMenuOptions();

	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->menuSinglePlayer);
	SAFE_DELETE(this->menu);

	return 0;
}

// This is a LOCAL FUNCTION that asks the user for a profile.
//
// It's a big hack, I know.
// Blocks the program completely.

#include <cstring>
#include <cstdlib>

std::string getProfileName(Window* main)
{
	// Direct ncurses calls?
	// Are you kidding me?

	// Making everything "right"
	nocbreak();					// input requires <enter>
	curs_set(1);				// showing cursor
	echo();						// echoing typed characters
	nodelay(stdscr, FALSE);		// waiting for user

	Window window(main,
	              main->getW() / 3,
	              main->getH() / 2 - 10/2,
	              main->getW() / 3,
	              10);

	if (Globals::Profiles::current->settings.screen.show_borders)
	{
		window.borders(Globals::Profiles::current->settings.screen.fancy_borders ?
		               Window::BORDER_FANCY :
		               Window::BORDER_REGULAR);
	}
	window.setTitle("New Profile");

	window.clear();
	window.print("Please enter a name:",
	             1,
	             1);

	window.print("(default: " +
	             Utils::File::getUser() +
	             ")",
	             1,
	             4,
	             Globals::Profiles::current->settings.theme.hilite_text);

	window.print_multiline("It may not contain the\n"
	                       "following characters:\n"
	                       "\\ / . ^ ; # = ~",
	                       1,
	                       6,
	                       0);


	window.refresh();


	char name[256];
	std::memset(name, '\0', 256);

	mvgetnstr(main->getY() + window.getY() + 3,
	          main->getX() + window.getX() + 1,
	          name, 255);

	std::string string_name(name);

	if (Profile::isNameValid(string_name))
	{
		if (string_name.empty())
			string_name = Utils::File::getUser();
	}

	// Returning to ncurses' "wrong" mode
	nodelay(stdscr, TRUE);		// won't wait at all
	noecho();					// dont echo typed chars
	curs_set(0);				// hiding the cursor
	cbreak();					// input dont require <enter>

	return string_name;
}

GameState::StateCode GameStateMainMenu::update()
{
	if (InputManager::isPressed("quit"))
		return GameState::QUIT;

	if (this->menuSinglePlayerActivated)
	{
		this->menuSinglePlayer->handleInput();

		if (this->menuSinglePlayer->willQuit())
		{
			saveSettingsMenuSinglePlayer();

			// And then exit based on the selected option.
			switch (this->menuSinglePlayer->currentID())
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
	else if (this->menuOptionsActvated)
	{
		this->menuOptions->handleInput();

		if (this->menuOptions->willQuit())
		{
			switch(this->menuOptions->currentID())
			{
			case GO_BACK:
				this->menuOptionsActvated = false;

				// Redrawing the screen to refresh settings
				saveSettingsMenuOptions();
				this->layout->windowsExit();
				this->layout->windowsInit();
				break;
			}
			this->menuOptions->reset();
		}
	}
	else if (this->menuProfilesActivated)
	{
		if (InputManager::isPressed((int)'d') ||
		    InputManager::isPressed((int)'D'))
		{
			std::string name = this->menuProfiles->currentLabel();

			// Only deleting profile if it's not the current one
			if (!(name == Globals::Profiles::current->name))
			{
				Profile::remove(name);
				this->menuProfiles->removeByLabel(name);
			}
		}
		else if (InputManager::isPressed((int)'s') ||
		         InputManager::isPressed((int)'S'))
		{
			// Switch to profile
			std::string name = this->menuProfiles->currentLabel();

			if (name != Globals::Profiles::current->name)
			{
				saveSettingsMenuSinglePlayer();
				saveSettingsMenuOptions();
				Globals::Profiles::current->saveSettings();

				delete Globals::Profiles::current;
				Globals::Profiles::current = new Profile(name);

				Globals::Profiles::current->loadSettings();

				// Re-create menus based on current settings
				createSinglePlayerMenu();
				createOptionsMenu();

				// Resetting the title name with current
				// profile name with an "'s" appended
				// (like "Rachel's" or "Chris'")
				if (name.back() == 's')
					name += '\'';
				else
					name += "'s";

				// Redrawing screen based on user's settings
				this->layout->logo->setTitle(name);
				this->layout->windowsExit();
				this->layout->windowsInit();
			}
		}
		else if (InputManager::isPressed((int)'c') ||
		         InputManager::isPressed((int)'C'))
		{
			std::string name = getProfileName(this->layout->main);

			if (name != Globals::Profiles::current->name)
				Profile::create(name);

			MenuItem* item = new MenuItem(name, profileMenuIndex);

			this->menuProfiles->add(item);
			++profileMenuIndex;
		}
		else
			this->menuProfiles->handleInput();

		if (this->menuProfiles->willQuit())
		{
			if (this->menuProfiles->currentID() == GO_BACK)
				this->menuProfilesActivated = false;


			this->menuProfiles->reset();
		}
	}
	else
	{
		// We're still at the Main Menu
		this->menu->handleInput();

		if (this->menu->willQuit())
		{
			switch(this->menu->currentID())
			{
			case SINGLE_PLAYER:
				this->menuSinglePlayerActivated = true;
				break;

			case OPTIONS:
				this->menuOptionsActvated = true;
				break;

			case PROFILES:
				this->menuProfilesActivated = true;
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

	else if (this->menuOptionsActvated)
		this->layout->draw(this->menuOptions);

	else if (this->menuProfilesActivated)
		this->layout->draw(this->menuProfiles);

	else
		this->layout->draw(this->menu);
}

void GameStateMainMenu::createMainMenu()
{
	SAFE_DELETE(this->menu);

	// Creating the Menu and Items.
	// Their default ids will be based on current Profile's
	// settings.
	this->menu = new Menu(1,
	                      1,
	                      this->layout->menu->getW() - 2,
	                      this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Single Player", SINGLE_PLAYER);
	menu->add(item);

	item = new MenuItem("Options", OPTIONS);
	menu->add(item);

	item = new MenuItem("Profiles", PROFILES);
	menu->add(item);

	item = new MenuItem("Quit", QUIT_GAME);
	menu->add(item);
}
void GameStateMainMenu::createSinglePlayerMenu()
{
	SAFE_DELETE(this->menuSinglePlayer);

	this->menuSinglePlayer = new Menu(1,
	                                  1,
	                                  this->layout->menu->getW() - 2,
	                                  this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Start Game", START_GAME);
	menuSinglePlayer->add(item);

	item = new MenuItem("Back", GO_BACK);
	menuSinglePlayer->add(item);

	menuSinglePlayer->addBlank();

	MenuItemNumberbox* number;

	number = new MenuItemNumberbox("Starting Level", STARTING_LEVEL, 1, 22, Globals::Profiles::current->settings.game.starting_level);
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

	this->menuOptions = new Menu(1,
	                             1,
	                             this->layout->menu->getW() - 2,
	                             this->layout->menu->getH() - 2);

	item = new MenuItem("Back", GO_BACK);
	menuOptions->add(item);
}
void GameStateMainMenu::createOptionsMenu()
{
	SAFE_DELETE(this->menuOptions);

	this->menuOptions = new Menu(1,
	                             1,
	                             this->layout->menu->getW() - 2,
	                             this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Back", GO_BACK);
	menuOptions->add(item);

	menuOptions->addBlank();

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Show Borders",
	                             SHOW_BORDERS,
	                             Globals::Profiles::current->settings.screen.show_borders);
	menuOptions->add(check);

	check = new MenuItemCheckbox("Fancy Borders",
	                             FANCY_BORDERS,
	                             Globals::Profiles::current->settings.screen.fancy_borders);
	menuOptions->add(check);

	check = new MenuItemCheckbox("Outer Border",
	                             OUTER_BORDER,
	                             Globals::Profiles::current->settings.screen.outer_border);
	menuOptions->add(check);

	check = new MenuItemCheckbox("Center Horizontal",
	                             CENTER_HORIZONTAL,
	                             Globals::Profiles::current->settings.screen.center_horizontally);
	menuOptions->add(check);

	check = new MenuItemCheckbox("Center Vertical",
	                             CENTER_VERTICAL,
	                             Globals::Profiles::current->settings.screen.center_vertically);
	menuOptions->add(check);

	check = new MenuItemCheckbox("Show Statistics",
	                             SHOW_STATISTICS,
	                             Globals::Profiles::current->settings.screen.show_statistics);
	menuOptions->add(check);

	MenuItemNumberbox* box;

	box = new MenuItemNumberbox("Next Pieces",
	                            NEXT_PIECES,
	                            1,
	                            7,
	                            Globals::Profiles::current->settings.game.next_pieces);
	menuOptions->add(box);

	std::vector<std::string> options;
	options.push_back("regular");
	options.push_back("dumb");

	MenuItemTextlist* list;

	list = new MenuItemTextlist("Random Piece",
	                            RANDOM_ALGORITHM,
	                            options,
	                            Globals::Profiles::current->settings.game.random_algorithm);
	menuOptions->add(list);

	box = new MenuItemNumberbox("Line clear delay(ms)",
	                            LINE_DELAY,
	                            0,
	                            300,
	                            Globals::Profiles::current->settings.game.line_clear_delay);
	menuOptions->add(box);
}
void GameStateMainMenu::createProfilesMenu()
{
	SAFE_DELETE(this->menuProfiles);

	this->menuProfiles = new Menu(1,
	                              4,
	                              this->layout->menu->getW() - 2,
	                              this->layout->menu->getH() - 2 - 3);

	menuProfiles->addBlank();

	MenuItem* item;

	item = new MenuItem("Back", GO_BACK);
	menuProfiles->add(item);

	menuProfiles->addBlank();

	// This seems kinda complicated, but we're dealing with
	// two indexes here.
	//
	// We're adding profiles to the menu based on the ones
	// that exist WHILE keeping account of them,
	// becase the user might want to create/delete profiles
	// later.
	unsigned int i = 0;

	for (profileMenuIndex = 0; i < (Profile::profiles.size()); profileMenuIndex++, i++)
	{
		item = new MenuItem(Profile::profiles[i], PROFILES_NAME + profileMenuIndex);
		menuProfiles->add(item);
	}
}
void GameStateMainMenu::saveSettingsMenuOptions()
{
	if (!this->menuOptions)
		return;

	// Alias
	Profile* current = Globals::Profiles::current;

	// User selected an option
	// Let's get ids from menu items
	current->settings.screen.show_borders        = this->menuOptions->getBool(SHOW_BORDERS);
	current->settings.screen.fancy_borders       = this->menuOptions->getBool(FANCY_BORDERS);
	current->settings.screen.outer_border        = this->menuOptions->getBool(OUTER_BORDER);
	current->settings.screen.center_horizontally = this->menuOptions->getBool(CENTER_HORIZONTAL);
	current->settings.screen.center_vertically   = this->menuOptions->getBool(CENTER_VERTICAL);
	current->settings.screen.show_statistics     = this->menuOptions->getBool(SHOW_STATISTICS);
	current->settings.game.next_pieces           = this->menuOptions->getInt(NEXT_PIECES);
	current->settings.game.random_algorithm      = this->menuOptions->getString(RANDOM_ALGORITHM);
	current->settings.game.line_clear_delay      = this->menuOptions->getInt(LINE_DELAY);
}
void GameStateMainMenu::saveSettingsMenuSinglePlayer()
{
	if (!this->menuSinglePlayer)
		return;

	// Alias
	Profile* current = Globals::Profiles::current;

	// User selected an option
	// Let's get ids from menu items
	current->settings.game.initial_noise  = this->menuSinglePlayer->getInt(INITIAL_NOISE);
	current->settings.game.starting_level = (unsigned int)this->menuSinglePlayer->getInt(STARTING_LEVEL);

	current->settings.game.invisible   = this->menuSinglePlayer->getBool(INVISIBLE);
	current->settings.game.slide_left  = this->menuSinglePlayer->getBool(SLIDE_LEFT);
	current->settings.game.slide_right = this->menuSinglePlayer->getBool(SLIDE_RIGHT);
	current->settings.game.can_hold    = this->menuSinglePlayer->getBool(HOLD_PIECE);
	current->settings.game.has_ghost   = this->menuSinglePlayer->getBool(GHOST_PIECE);
}



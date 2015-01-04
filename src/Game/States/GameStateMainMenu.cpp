#include <Game/States/GameStateMainMenu.hpp>
#include <Game/States/GameStateGame.hpp>
#include <Game/Entities/Profile.hpp>
#include <Engine/InputManager.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/Graphics/Widgets/Dialog.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/File.hpp>
#include <Engine/Helpers/String.hpp>
#include <Game/Config/Globals.hpp>
#include <Engine/Flow/StateManager.hpp>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	// Main Menu
	SINGLE_PLAYER,
	GAME_SETTINGS,
	GUI_OPTIONS,
	CONTROLS,
	PROFILES,
	HELP,
	QUIT_GAME,

	GO_BACK,

	// Single Player Submenu
	START_GAME,
	STARTING_LEVEL,
	INITIAL_NOISE,
	INVISIBLE,
	SLIDE_LEFT,
	SLIDE_RIGHT,
	HOLD_PIECE,
	GHOST_PIECE,

	// Game Settings
	SHOW_STATISTICS,
	RANDOM_ALGORITHM,
	LINE_DELAY,
	NEXT_PIECES,

	// GUI Options
	SHOW_BORDERS,
	FANCY_BORDERS,
	OUTER_BORDER,
	USE_COLORS,
	CENTER_HORIZONTAL,
	CENTER_VERTICAL,
	ANIMATION_MENU,
	ANIMATION_GAME,

	// Controls Submenu
	CONTROLS_KEY_LEFT,
	CONTROLS_KEY_RIGHT,
	CONTROLS_KEY_DOWN,
	CONTROLS_KEY_DROP,
	CONTROLS_KEY_ROTATE_CLOCKWISE,
	CONTROLS_KEY_ROTATE_COUNTERCLOCKWISE,
	CONTROLS_KEY_ROTATE_180,
	CONTROLS_KEY_PAUSE,
	CONTROLS_KEY_HELP,
	CONTROLS_KEY_HOLD,
	CONTROLS_KEY_QUIT,
	CONTROLS_DEFAULT,

	// Profiles Submenu
	PROFILES_NAME
};

// I need this so when we add/remove/change profiles
// from the menu things don't get messed up.
unsigned int profileMenuIndex = 0;

GameStateMainMenu::GameStateMainMenu():
	layout(NULL),
	menu(NULL),
	menuSinglePlayer(NULL),
	menuSinglePlayerActivated(false),
	menuGameSettings(NULL),
	menuGameSettingsActivated(false),
	menuGUIOptions(NULL),
	menuGUIOptionsActivated(false),
	menuProfiles(NULL),
	menuProfilesActivated(false),
	menuControls(NULL),
	menuControlsActivated(false),
	helpWindows(NULL)
{ }
GameStateMainMenu::~GameStateMainMenu()
{ }
void GameStateMainMenu::load()
{
	this->layout = new LayoutMainMenu(80, 24, this);

	createMainMenu();
	createSinglePlayerMenu();
	createGameSettingsMenu();
	createGUIOptionsMenu();
	createProfilesMenu();
	createControlsMenu();

	this->helpWindows = new WindowGameHelp();
}

void GameStateMainMenu::unload()
{
	saveSettingsMenuSinglePlayer();
	saveSettingsMenuOptions();

	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->menuProfiles);
	SAFE_DELETE(this->menuControls);
	SAFE_DELETE(this->menuGameSettings);
	SAFE_DELETE(this->menuGUIOptions);
	SAFE_DELETE(this->menuSinglePlayer);
	SAFE_DELETE(this->menu);
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

	window.print(Utils::String::split("It may not contain the\n"
	                                  "following characters:\n"
	                                  "\\ / . ^ ; # = ~", '\n'),
	             1,
	             6);

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

void GameStateMainMenu::update()
{
	if (InputManager::isPressed("quit"))
		StateManager::quit();

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
				StateManager::change(new GameStateGame());
				break;

			case GO_BACK:
				this->menuSinglePlayerActivated = false;
				break;
			}
			this->menuSinglePlayer->reset();
		}
	}
	else if (this->menuGameSettingsActivated)
	{
		this->menuGameSettings->handleInput();

		if (this->menuGameSettings->willQuit())
		{
			switch(this->menuGameSettings->currentID())
			{
			case GO_BACK:
				this->menuGameSettingsActivated = false;
			}
			this->menuGameSettings->reset();
		}
	}
	else if (this->menuGUIOptionsActivated)
	{
		this->menuGUIOptions->handleInput();

		if (this->menuGUIOptions->willQuit())
		{
			switch(this->menuGUIOptions->currentID())
			{
			case GO_BACK:
				this->menuGUIOptionsActivated = false;

				// Redrawing the screen to refresh settings
				saveSettingsMenuOptions();
				this->layout->windowsExit();
				this->layout->windowsInit();
				break;
			}
			this->menuGUIOptions->reset();
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
				Globals::Profiles::current->applyGraphicalSettings();

				// Re-create menus based on current settings
				createSinglePlayerMenu();
				createGameSettingsMenu();
				createGUIOptionsMenu();
				createControlsMenu();

				// Resetting the title name with current
				// profile name with an "'s" appended
				// (like "Rachel's" or "Chris'")
				if (Utils::String::back(name) == 's')
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
	else if (this->menuControlsActivated)
	{
		this->menuControls->handleInput();

		if (this->menuControls->willQuit())
		{
			std::string key(""); // for key binding

			switch(this->menuControls->currentID())
			{
			case GO_BACK:
				this->menuControlsActivated = false;
				break;

			case CONTROLS_KEY_LEFT:  key = "left";  break;
			case CONTROLS_KEY_RIGHT: key = "right"; break;
			case CONTROLS_KEY_DOWN:  key = "down";  break;
			case CONTROLS_KEY_DROP:  key = "drop";  break;
			case CONTROLS_KEY_ROTATE_CLOCKWISE:        key = "rotate_clockwise"; break;
			case CONTROLS_KEY_ROTATE_COUNTERCLOCKWISE: key = "rotate_counterclockwise"; break;
			case CONTROLS_KEY_ROTATE_180:              key = "rotate_180"; break;
			case CONTROLS_KEY_PAUSE: key  = "pause";  break;
			case CONTROLS_KEY_HOLD:  key  = "hold";  break;
			case CONTROLS_KEY_HELP:  key  = "help";  break;
			case CONTROLS_KEY_QUIT:  key  = "quit";  break;

			case CONTROLS_DEFAULT:
			{
				Globals::Profiles::current->resetKeybindings();

				// Resetting the menu to show the new labels
				createControlsMenu();
				menuControls->goLast();
				break;
			}
			}

			// If we'll change a key binding
			if (! key.empty())
			{
				Dialog::show("Press any key, Enter to Cancel");
				int tmp = Ncurses::getInput(-1);

				if ((tmp != KEY_ENTER) &&
				    (tmp != '\n') &&
				    (tmp != ERR) &&
				    (! InputManager::isBound(tmp)))
				{
					InputManager::bind(key, tmp);

					MenuItemLabel* label;
					label = (MenuItemLabel*)menuControls->current;

					label->set(InputManager::keyToString(tmp));
				}
			}
			this->menuControls->reset();
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

			case HELP:
				this->helpWindows->run();
				break;

			case GAME_SETTINGS:
				this->menuGameSettingsActivated = true;
				break;

			case GUI_OPTIONS:
				this->menuGUIOptionsActivated = true;
				break;

			case CONTROLS:
				this->menuControlsActivated = true;
				break;

			case PROFILES:
				this->menuProfilesActivated = true;
				break;

			case QUIT_GAME:
				StateManager::quit();
				break;
			}
			this->menu->reset();
		}
	}
}

void GameStateMainMenu::draw()
{
	if (this->menuSinglePlayerActivated)
		this->layout->draw(this->menuSinglePlayer);

	else if (this->menuGameSettingsActivated)
		this->layout->draw(this->menuGameSettings);

	else if (this->menuGUIOptionsActivated)
		this->layout->draw(this->menuGUIOptions);

	else if (this->menuControlsActivated)
		this->layout->draw(this->menuControls);

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

	item = new MenuItem("Game Settings", GAME_SETTINGS);
	menu->add(item);

	item = new MenuItem("GUI Options", GUI_OPTIONS);
	menu->add(item);

	item = new MenuItem("Controls", CONTROLS);
	menu->add(item);

	item = new MenuItem("Profiles", PROFILES);
	menu->add(item);

	item = new MenuItem("Help", HELP);
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

	number = new MenuItemNumberbox("Starting Level", STARTING_LEVEL, 1, 22, Globals::Profiles::current->scores->score.starting_level);
	menuSinglePlayer->add(number);

	number = new MenuItemNumberbox("Initial Noise", INITIAL_NOISE, 0, 20, Globals::Profiles::current->scores->score.initial_noise);
	menuSinglePlayer->add(number);

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Invisible",
	                             INVISIBLE,
	                             Globals::Profiles::current->scores->score.invisible);
	menuSinglePlayer->add(check);

	check = new MenuItemCheckbox("Slide Left",
	                             SLIDE_LEFT,
	                             Globals::Profiles::current->scores->score.slide_left);
	menuSinglePlayer->add(check);

	check = new MenuItemCheckbox("Slide Right",
	                             SLIDE_RIGHT,
	                             Globals::Profiles::current->scores->score.slide_right);
	menuSinglePlayer->add(check);

	check = new MenuItemCheckbox("Hold Piece",
	                             HOLD_PIECE,
	                             Globals::Profiles::current->settings.game.can_hold);
	menuSinglePlayer->add(check);

	check = new MenuItemCheckbox("Ghost Piece",
	                             GHOST_PIECE,
	                             Globals::Profiles::current->settings.game.has_ghost);
	menuSinglePlayer->add(check);
}
void GameStateMainMenu::createGameSettingsMenu()
{
	SAFE_DELETE(this->menuGameSettings);

	this->menuGameSettings = new Menu(1,
	                                  1,
	                                  this->layout->menu->getW() - 2,
	                                  this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Back", GO_BACK);
	menuGameSettings->add(item);

	menuGameSettings->addBlank();

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Show Statistics",
	                             SHOW_STATISTICS,
	                             Globals::Profiles::current->settings.screen.show_statistics);
	menuGameSettings->add(check);

	MenuItemNumberbox* box;

	box = new MenuItemNumberbox("Next Pieces",
	                            NEXT_PIECES,
	                            1,
	                            7,
	                            Globals::Profiles::current->settings.game.next_pieces);
	menuGameSettings->add(box);

	std::vector<std::string> options;
	options.push_back("regular");
	options.push_back("dumb");

	MenuItemTextlist* list;

	list = new MenuItemTextlist("Random Piece",
	                            RANDOM_ALGORITHM,
	                            options,
	                            Globals::Profiles::current->scores->score.random_algorithm);
	menuGameSettings->add(list);

	box = new MenuItemNumberbox("Line clear delay(ms)",
	                            LINE_DELAY,
	                            0,
	                            300,
	                            Globals::Profiles::current->settings.game.line_clear_delay);
	menuGameSettings->add(box);
}
void GameStateMainMenu::createGUIOptionsMenu()
{
	SAFE_DELETE(this->menuGUIOptions);

	this->menuGUIOptions = new Menu(1,
	                                1,
	                                this->layout->menu->getW() - 2,
	                                this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Back", GO_BACK);
	menuGUIOptions->add(item);

	menuGUIOptions->addBlank();

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Show Borders",
	                             SHOW_BORDERS,
	                             EngineGlobals::Screen::show_borders);
	menuGUIOptions->add(check);

	check = new MenuItemCheckbox("Fancy Borders",
	                             FANCY_BORDERS,
	                             EngineGlobals::Screen::fancy_borders);
	menuGUIOptions->add(check);

	check = new MenuItemCheckbox("Outer Border",
	                             OUTER_BORDER,
	                             EngineGlobals::Screen::outer_border);
	menuGUIOptions->add(check);

	check = new MenuItemCheckbox("Center Horizontal",
	                             CENTER_HORIZONTAL,
	                             EngineGlobals::Screen::center_horizontally);
	menuGUIOptions->add(check);

	check = new MenuItemCheckbox("Center Vertical",
	                             CENTER_VERTICAL,
	                             EngineGlobals::Screen::center_vertically);
	menuGUIOptions->add(check);

	// Animation on the background of the menu and game
	std::vector<std::string> options;
	options.push_back("random");
	options.push_back("fire");
	options.push_back("water");
	options.push_back("snakes");
	options.push_back("life");
	options.push_back("none");

	MenuItemTextlist* list;

	list = new MenuItemTextlist("Menu Animation",
	                            ANIMATION_MENU,
	                            options,
	                            Globals::Profiles::current->settings.screen.animation_menu);
	menuGUIOptions->add(list);

	list = new MenuItemTextlist("Game Animation",
	                            ANIMATION_GAME,
	                            options,
	                            Globals::Profiles::current->settings.screen.animation_game);
	menuGUIOptions->add(list);
}
void GameStateMainMenu::createControlsMenu()
{
	SAFE_DELETE(this->menuControls);

	this->menuControls = new Menu(1,
	                              1,
	                              this->layout->menu->getW() - 2,
	                              this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Back", GO_BACK);
	menuControls->add(item);

	menuControls->addBlank();

	MenuItemLabel* label;
	std::string str;

	str = InputManager::keyToString(InputManager::getBind("left"));
	label = new MenuItemLabel("Move Left", CONTROLS_KEY_LEFT, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("right"));
	label = new MenuItemLabel("Move Right", CONTROLS_KEY_RIGHT, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("down"));
	label = new MenuItemLabel("Soft Drop", CONTROLS_KEY_DOWN, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("drop"));
	label = new MenuItemLabel("Hard Drop", CONTROLS_KEY_DROP, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("rotate_clockwise"));
	label = new MenuItemLabel("Clockwise", CONTROLS_KEY_ROTATE_CLOCKWISE, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("rotate_counterclockwise"));
	label = new MenuItemLabel("Counter-Clockwise", CONTROLS_KEY_ROTATE_COUNTERCLOCKWISE, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("rotate_180"));
	label = new MenuItemLabel("Rotate by 180", CONTROLS_KEY_ROTATE_180, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("pause"));
	label = new MenuItemLabel("Pause", CONTROLS_KEY_PAUSE, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("hold"));
	label = new MenuItemLabel("Hold Piece", CONTROLS_KEY_HOLD, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("help"));
	label = new MenuItemLabel("Show Help", CONTROLS_KEY_HELP, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("quit"));
	label = new MenuItemLabel("Quit Game", CONTROLS_KEY_QUIT, str);
	menuControls->add(label);

	menuControls->addBlank();

	item = new MenuItem("Reset to Defaults", CONTROLS_DEFAULT);
	menuControls->add(item);
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
	if (!this->menuGUIOptions)
		return;

	// Alias
	Profile* current = Globals::Profiles::current;

	// User selected an option
	// Let's get ids from menu items
	current->settings.screen.show_borders        = this->menuGUIOptions->getBool(SHOW_BORDERS);
	current->settings.screen.fancy_borders       = this->menuGUIOptions->getBool(FANCY_BORDERS);
	current->settings.screen.outer_border        = this->menuGUIOptions->getBool(OUTER_BORDER);
	current->settings.screen.center_horizontally = this->menuGUIOptions->getBool(CENTER_HORIZONTAL);
	current->settings.screen.center_vertically   = this->menuGUIOptions->getBool(CENTER_VERTICAL);
	current->settings.screen.animation_menu      = this->menuGUIOptions->getString(ANIMATION_MENU);
	current->settings.screen.animation_game      = this->menuGUIOptions->getString(ANIMATION_GAME);
	current->applyGraphicalSettings();

	current->settings.screen.show_statistics = this->menuGameSettings->getBool(SHOW_STATISTICS);
	current->settings.game.next_pieces       = this->menuGameSettings->getInt(NEXT_PIECES);
	current->scores->score.random_algorithm  = this->menuGameSettings->getString(RANDOM_ALGORITHM);
	current->settings.game.line_clear_delay  = this->menuGameSettings->getInt(LINE_DELAY);
}
void GameStateMainMenu::saveSettingsMenuSinglePlayer()
{
	if (!this->menuSinglePlayer)
		return;

	// Alias
	Profile* current = Globals::Profiles::current;

	// User selected an option
	// Let's get ids from menu items
	current->scores->score.initial_noise  = this->menuSinglePlayer->getInt(INITIAL_NOISE);
	current->scores->score.starting_level = this->menuSinglePlayer->getInt(STARTING_LEVEL);
	current->scores->score.level = current->scores->score.starting_level;

	current->scores->score.invisible   = this->menuSinglePlayer->getBool(INVISIBLE);
	current->scores->score.slide_left  = this->menuSinglePlayer->getBool(SLIDE_LEFT);
	current->scores->score.slide_right = this->menuSinglePlayer->getBool(SLIDE_RIGHT);
	current->settings.game.can_hold    = this->menuSinglePlayer->getBool(HOLD_PIECE);
	current->settings.game.has_ghost   = this->menuSinglePlayer->getBool(GHOST_PIECE);
}



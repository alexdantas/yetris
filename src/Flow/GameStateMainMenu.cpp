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

	// Creating the Menu and Items
	this->menu = new Menu(1,
	                      1,
	                      this->layout->menu->getW() - 2,
	                      this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Start Game", START_GAME);
	menu->add(item);

	menu->addBlank();

	MenuItemNumberbox* number;

	number = new MenuItemNumberbox("Starting Level", STARTING_LEVEL, 1, 18, 1);
	menu->add(number);

	number = new MenuItemNumberbox("Initial Noise", INITIAL_NOISE, 0, 20, 0);
	menu->add(number);

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Invisible", INVISIBLE);
	menu->add(check);

	check = new MenuItemCheckbox("Slide Left", SLIDE_LEFT);
	menu->add(check);

	check = new MenuItemCheckbox("Slide Right", SLIDE_RIGHT);
	menu->add(check);

	check = new MenuItemCheckbox("Hold Piece", HOLD_PIECE, true);
	menu->add(check);

	check = new MenuItemCheckbox("Ghost Piece", GHOST_PIECE, true);
	menu->add(check);

	check = new MenuItemCheckbox("Show Statistics", SHOW_STATISTICS, true);
	menu->add(check);
}

int GameStateMainMenu::unload()
{
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
		Globals::Game::initial_noise = this->menu->getInt(INITIAL_NOISE);
		Globals::Game::starting_level = this->menu->getInt(STARTING_LEVEL);

		Globals::Game::invisible   = this->menu->getBool(INVISIBLE);
		Globals::Game::slide_left  = this->menu->getBool(SLIDE_LEFT);
		Globals::Game::slide_right = this->menu->getBool(SLIDE_RIGHT);
		Globals::Game::can_hold    = this->menu->getBool(HOLD_PIECE);
		Globals::Game::can_hold    = this->menu->getBool(HOLD_PIECE);
		Globals::Game::has_ghost   = this->menu->getBool(GHOST_PIECE);
		Globals::Screen::show_statistics = this->menu->getBool(SHOW_STATISTICS);

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



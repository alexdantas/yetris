#include <Flow/GameStateMainMenu.hpp>
#include <Interface/Ncurses.hpp>
#include <Misc/Utils.hpp>

GameStateMainMenu::GameStateMainMenu():
	layout(NULL),
	gameMode(0),
	menu(NULL)
{ }
GameStateMainMenu::~GameStateMainMenu()
{ }
void GameStateMainMenu::load(int stack)
{
	UNUSED(stack);

	this->layout = new LayoutMainMenu(80, 24);


	// Creating the Menu and Items
	this->menu = new Menu(34, 7, 10, 5);

	MenuItem* item;

	item = new MenuItem("Survival", 1);
	menu->add(item);

	item = new MenuItem("Invisible", 2);
	menu->add(item);

	item = new MenuItem("Slide Left", 3);
	menu->add(item);

	item = new MenuItem("Slide Right", 4);
	menu->add(item);
}

int GameStateMainMenu::unload()
{
	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->menu);

	return this->gameMode;
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

		// And then exit based on the selected option.
		switch(this->menu->getSelectedValue())
		{
		case 1:
			this->gameMode = 1;
			return GameState::GAME_START;

		case 2:
			this->gameMode = 2;
			return GameState::GAME_START;

		case 3:
			this->gameMode = 3;
			return GameState::GAME_START;

		case 4:
			this->gameMode = 4;
			return GameState::GAME_START;
		}
	}

	return GameState::CONTINUE;
}

void GameStateMainMenu::draw()
{
	this->layout->draw(this->menu);
}



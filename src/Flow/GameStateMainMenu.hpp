#ifndef GAMESTATEMAINMENU_H_DEFINED
#define GAMESTATEMAINMENU_H_DEFINED

#include <Flow/GameState.hpp>
#include <Interface/LayoutMainMenu.hpp>
#include <Interface/Menu.hpp>

/// The Main Menu.
///
class GameStateMainMenu: public GameState
{
public:
	GameStateMainMenu();
	virtual ~GameStateMainMenu();

    void load(int stack=0);

    int unload();

	GameState::StateCode update();

    void draw();

private:
	LayoutMainMenu* layout;

	/// The actual items on the menu.
	Menu* menu;
};

#endif //GAMESTATEMAINMENU_H_DEFINED


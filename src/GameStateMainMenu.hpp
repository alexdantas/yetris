#ifndef GAMESTATEMAINMENU_H_DEFINED
#define GAMESTATEMAINMENU_H_DEFINED

#include "GameState.hpp"
#include "LayoutMainMenu.hpp"

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
};

#endif //GAMESTATEMAINMENU_H_DEFINED


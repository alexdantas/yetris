#ifndef GAMESTATEMAINMENU_H_DEFINED
#define GAMESTATEMAINMENU_H_DEFINED

#include <Engine/Flow/GameState.hpp>
#include <Game/Display/Layouts/LayoutMainMenu.hpp>
#include <Engine/Graphics/Widgets/Menu.hpp>
#include <Game/Display/WindowGameHelp.hpp>

/// The Main Menu.
///
class GameStateMainMenu: public GameState
{
	friend class LayoutMainMenu;

public:
	GameStateMainMenu();
	virtual ~GameStateMainMenu();

    void load(int stack=0);

    int unload();

	GameState::StateCode update();

    void draw();

private:
	LayoutMainMenu* layout;

	/// The main menu.
	Menu* menu;

	/// Single Player submenu.
	Menu* menuSinglePlayer;
	bool menuSinglePlayerActivated;

	Menu* menuOptions;
	bool menuOptionsActvated;

	Menu* menuProfiles;
	bool menuProfilesActivated;

	Menu* menuControls;
	bool menuControlsActivated;

	WindowGameHelp* helpWindows;

	// easily create internal menus
	void createMainMenu();
	void createSinglePlayerMenu();
	void createOptionsMenu();
	void createControlsMenu();
	void createProfilesMenu();
	void saveSettingsMenuOptions();
	void saveSettingsMenuSinglePlayer();
};

#endif //GAMESTATEMAINMENU_H_DEFINED


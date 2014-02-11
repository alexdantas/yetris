#ifndef GAMESTATEFIRSTTIME_H_DEFINED
#define GAMESTATEFIRSTTIME_H_DEFINED

#include <Flow/GameState.hpp>
#include <Interface/LayoutFirstTime.hpp>
#include <Interface/Menu.hpp>

/// Will appear only when there's no Profile at the
/// configuration directory - probably the first time
/// the game's being run.
///
/// This asks for a Profile name and initializes
/// all configuration files for it.
///
class GameStateFirstTime: public GameState
{
public:
	GameStateFirstTime();
	virtual ~GameStateFirstTime();

    void load(int stack=0);

    int unload();

	GameState::StateCode update();

    void draw();


private:
	LayoutFirstTime* layout;

	Menu* menu;
};

#endif //GAMESTATEFIRSTTIME_H_DEFINED


#ifndef GAMESTATEFIRSTTIME_H_DEFINED
#define GAMESTATEFIRSTTIME_H_DEFINED

#include <Engine/Flow/GameState.hpp>
#include <Game/Display/Layouts/LayoutFirstTime.hpp>
#include <Engine/Graphics/Widgets/Menu.hpp>

#include <string>

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

    void load();

    void unload();

	void update();

    void draw();


private:
	LayoutFirstTime* layout;

	/// Where we store the Profile name we query the user.
	///
	/// This is a big hack so we can ask the user for
	/// a name to enter as a profile.
	///
	/// It exists because to create a Textbox I need a
	/// default color to print it.
	/// Since the default color is specified by the
	/// Profile (and right now we have no Profiles)
	/// I need to call raw ncurses functions and do other
	/// nasty things.
	///
	/// Check out the implementation.
	std::string name;
};

#endif //GAMESTATEFIRSTTIME_H_DEFINED


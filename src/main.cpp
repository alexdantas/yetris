#include <Engine/Flow/StateManager.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Game/Config/Globals.hpp>
#include <Game/Config/Arguments.hpp>
#include <Engine/Helpers/Utils.hpp>

int main(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	try
	{
		Globals::init();
		Arguments::parse(argc, argv);
		Utils::Random::seed();
		Ncurses::init();
		Colors::init();

		StateManager states;
		states.run();

		Ncurses::exit();
	}
	catch (...)
	{
		// I dont really have a nice exception-handling scheme right
		// now. I must learn how to properly deal with them.
		Ncurses::exit();
		return 666;
	}

	return 0;
}


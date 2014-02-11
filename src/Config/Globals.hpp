#ifndef GLOBALS_H_DEFINED
#define GLOBALS_H_DEFINED

#include <Game/Profile.hpp>

/// All global settings to the game.
///
namespace Globals
{
	/// Allocates necessary variables.
	///
	/// @note No need for Globals::exit() because the
	///       Operational System always frees the memory
	///       when quitting the program.
	///       And definitely this module will need to
	///       be accessed until the end of the program.
	///
	void init();

	// Accessing version numbers - version[MAJOR] for example
	#define MAJOR 0
	#define MINOR 1
	#define PATCH 2

	/// Game version (format MMP - Major Minor Patch)
	extern int version[3];

	namespace Profiles
	{
		extern Profile* current;
		extern std::string default_name;
	};
	namespace Config
	{
		/// Root directory where we place configurations.
		///
		/// It's `~/.local/share/yetris/`.
		///
		/// @note It has a trailing '/'.
		extern std::string directory;

		/// Main file where global settings reside.
		///
		/// It's `~/.local/share/yetris/global-settings.ini`.
		///
		/// @note Most settings are user-wide, according to
		///       it's Profile.
		extern std::string file;
	};
};

#endif //GLOBALS_H_DEFINED


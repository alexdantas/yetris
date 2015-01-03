#include <Game/Config/Globals.hpp>
#include <Engine/Helpers/INI.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/File.hpp>
#include <Engine/Helpers/String.hpp>

#include <ncurses.h>

// VERSION is formatted like "0.0.1" - i'm skipping the dots
char Globals::version[3] = { VERSION[0],
                             VERSION[2],
                             VERSION[4] };

//  ___   ___   ___   ____  _   _     ____  __
// | |_) | |_) / / \ | |_  | | | |   | |_  ( (`
// |_|   |_| \ \_\_/ |_|   |_| |_|__ |_|__ _)_)

Profile* Globals::Profiles::current = NULL;
std::string Globals::Profiles::default_name = "";

//  __    ___   _      ____  _   __
// / /`  / / \ | |\ | | |_  | | / /`_
// \_\_, \_\_/ |_| \| |_|   |_| \_\_/

// real initialization at init()
std::string Globals::Config::directory = "";
std::string Globals::Config::file      = "";

//  _   _      _  _____
// | | | |\ | | |  | |
// |_| |_| \| |_|  |_|

void Globals::init()
{
	// Making sure default config directory exists
	// By default it's `~/.local/share/yetris/`

	Globals::Config::directory = (Utils::File::getHome() +
	                              ".local/share/" +
	                              PACKAGE + "/");

	if (Utils::String::front(Globals::Config::directory) != '/')
	{
		// We couldn't get user's home directory,
		// so let's fallback to `/tmp/.local/share...`
		Globals::Config::directory = ("/tmp/" +
		                              Globals::Config::directory);
	}

	Globals::Config::file = (Globals::Config::directory +
	                         "global-settings.ini");

	if (! Utils::File::isDirectory(Globals::Config::directory))
		Utils::File::mkdir_p(Globals::Config::directory);

	if (! Utils::File::isDirectory(Globals::Config::directory))
	{
		// We REALLY can't access the disk by any means.
		// Let's throw everything away and give up.
		Globals::Config::directory = "/dev/";
		Globals::Config::file      = "/dev/null";
		return;
	}

	// Getting default profile name - should be at the
	// global settings file.
	if (Utils::File::exists(Globals::Config::file))
	{
		INI::Parser* ini = NULL;

		try {
			ini = new INI::Parser(Globals::Config::file);
		}
		catch(std::runtime_error& e)
		{
			// File doesn't exist (or we couldn't access it)
			// Either way, ignore it silently
			SAFE_DELETE(ini);
			return;
		}

		std::string buffer = (*ini)("profiles")["default"];

		if (! buffer.empty())
			Globals::Profiles::default_name	= buffer;
	}
}


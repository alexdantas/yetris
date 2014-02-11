#include <Game/Profile.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>
#include <Config/INI.hpp>
#include <Interface/Colors.hpp>

#include <vector>
#include <algorithm>

std::vector<std::string> Profile::profiles;

bool Profile::load()
{
	// Let's see if there's any profiles already on the system.
	// Profiles are defined by their directory on the
	// default configuration dir.

	// Getting all directories within
	// the default config dir.
	std::vector<std::string> files = Utils::File::ls(Globals::Config::directory);

	std::vector<std::string> dirs;
	for (unsigned int i = 0; i < files.size(); i++)
		if (Utils::File::isDirectory(files[i]))
		{
			// Sending the user name (last part of dir name)
			if (files[i].back() == '/')
				files[i].pop_back();
			size_t pos = files[i].rfind('/');

			dirs.push_back(files[i].substr(pos + 1));
		}

	if (dirs.empty())
	{
		// Woops, couldn't find any dir - therefore any user!
		// Is this your first time or is there an issue?
		return false;
	}

	// Finally, the list!
	std::sort(dirs.begin(), dirs.end());
	Profile::profiles = dirs;

	return true;
}

Profile::Profile(std::string name):
	name(name)
{
	std::string root   = Globals::Config::directory + name + "/";
	std::string config = root + "settings.ini";
	std::string stats  = root + "statistics.bin";

	// Make sure we can load a default profile at startup
	// (meaning the global settings file must have a profile
	//  name at `profiles:default`)

	if (! Utils::File::exists(Globals::Config::file))
	{
		INI ini;
		ini.create();
		ini.set("profiles:default", name);
		ini.save(Globals::Config::file);
	}
	else
	{
		INI ini;
		ini.load(Globals::Config::file);
		std::string default_name = ini.get("profiles:default", "");

		if (default_name.empty())
		{
			ini.set("profiles:default", name);
			ini.save(Globals::Config::file);
		}
	}


	// Making sure default directories and files exist.

	if (! Utils::File::isDirectory(root))
		Utils::File::mkdir_p(root);

	if (! Utils::File::exists(config))
		Utils::File::create(config);

	if (! Utils::File::exists(stats))
		Utils::File::create(stats);

	// Starting all settings with default, hardcoded values.

	// Screen

	settings.screen.center_horizontally = true;
	settings.screen.center_vertically   = true;

	settings.screen.show_borders  = true;
	settings.screen.fancy_borders = true;
	settings.screen.outer_border  = true;

	settings.screen.use_colors = true;

	settings.screen.show_statistics = true;

	// Game

	settings.game.next_pieces = 7;
	settings.game.initial_noise = 0;
	settings.game.starting_level = 1;

	settings.game.has_ghost = true;
	settings.game.can_hold  = true;

	settings.game.random_algorithm = 1;

	settings.game.has_game_over_animation = true;
	settings.game.line_clear_delay = 200; 		// ms

	settings.game.slide_left  = false;
	settings.game.slide_right = false;
	settings.game.invisible = false;

	// Theme

	settings.theme.text = Colors::pair(COLOR_WHITE, COLOR_DEFAULT);
	settings.theme.hilite_text = Colors::pair(COLOR_BLUE, COLOR_DEFAULT);

	settings.theme.piece_has_colors = true;
	settings.theme.ghost_has_colors = true;
	settings.theme.show_pivot_block = false;
	settings.theme.lock_piece_color = false;

	settings.theme.clear_line = new Block(Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
	                ':', ':');

	settings.theme.piece_colorless = new Block(Colors::pair(COLOR_BLACK, COLOR_WHITE),
	                ' ', ' ');

	settings.theme.piece = new Block(Colors::pair(COLOR_BLACK, COLOR_WHITE),
	                ' ', ' ');

	settings.theme.ghost = new Block(Colors::pair(COLOR_BLACK, COLOR_WHITE, true),
	                '[', ']');

	settings.theme.locked = new Block(Colors::pair(COLOR_WHITE, COLOR_WHITE),
	                ' ', ' ');

	settings.theme.invisible = new Block(Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
	                                      ' ', ' ');

	settings.theme.piece_S = new Block(Colors::pair(COLOR_WHITE, COLOR_GREEN),
	                ' ', ' ');

	settings.theme.piece_Z = new Block(Colors::pair(COLOR_WHITE, COLOR_RED),
	                ' ', ' ');

	settings.theme.piece_O = new Block(Colors::pair(COLOR_WHITE, COLOR_YELLOW),
	                ' ', ' ');

	settings.theme.piece_I = new Block(Colors::pair(COLOR_WHITE, COLOR_CYAN, true),
	                ' ', ' ');

	settings.theme.piece_L = new Block(Colors::pair(COLOR_WHITE, COLOR_YELLOW, true),
	                ' ', ' ');

	settings.theme.piece_J = new Block(Colors::pair(COLOR_WHITE, COLOR_BLUE),
	                ' ', ' ');

	settings.theme.piece_T = new Block(Colors::pair(COLOR_WHITE, COLOR_MAGENTA),
	                ' ', ' ');

	// Input

	settings.input.left                    = KEY_LEFT;
	settings.input.right                   = KEY_RIGHT;
	settings.input.up                      = KEY_UP;
	settings.input.down                    = KEY_DOWN;
	settings.input.drop                    = ' ';
	settings.input.rotate_clockwise        = 'x';
	settings.input.rotate_counterclockwise = 'z';
	settings.input.pause                   = 'p';
	settings.input.hold                    = 'c';
	settings.input.toggle_statistics       = '2';
	settings.input.help                    = 'h';
	settings.input.high_scores             = '1';
	settings.input.quit                    = 'q';

	// Now will actuall load the files.
}
Profile::~Profile()
{
	SAFE_DELETE(settings.theme.clear_line);
	SAFE_DELETE(settings.theme.piece_colorless);
	SAFE_DELETE(settings.theme.piece);
	SAFE_DELETE(settings.theme.ghost);
	SAFE_DELETE(settings.theme.locked);
	SAFE_DELETE(settings.theme.invisible);

	SAFE_DELETE(settings.theme.piece_S);
	SAFE_DELETE(settings.theme.piece_Z);
	SAFE_DELETE(settings.theme.piece_O);
	SAFE_DELETE(settings.theme.piece_I);
	SAFE_DELETE(settings.theme.piece_L);
	SAFE_DELETE(settings.theme.piece_J);
	SAFE_DELETE(settings.theme.piece_T);
}


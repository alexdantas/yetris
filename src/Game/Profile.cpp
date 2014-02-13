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
bool Profile::isNameValid(std::string name)
{
	// Not allowed characters
	if ((name.find('/')  != std::string::npos) ||
	    (name.find('\\') != std::string::npos) ||
	    (name.find(';')  != std::string::npos) ||
	    (name.find('#')  != std::string::npos) ||
	    (name.find('=')  != std::string::npos) ||
	    (name.find('~')  != std::string::npos) ||
	    (name.find('.')  != std::string::npos))
		return false;

	return true;
}
void Profile::remove(std::string name)
{
	auto where = std::find(Profile::profiles.begin(), Profile::profiles.end(), name);

	if (where == Profile::profiles.end())
		return;

	Profile::profiles.erase(where);

	std::string dir = Globals::Config::directory + name;

	Utils::File::rm_rf(dir);
}
void Profile::create(std::string name)
{
	// Won't create existing profiles.
	if (std::find(Profile::profiles.begin(), Profile::profiles.end(), name) != Profile::profiles.end())
		return;

	Profile::profiles.push_back(name);
	std::sort(Profile::profiles.begin(), Profile::profiles.end());

	std::string dir = Globals::Config::directory + name;

	Utils::File::mkdir_p(dir);
}

Profile::Profile(std::string name):
	name(name),
	directory(Globals::Config::directory + name + "/"),
	fileSettings(directory + "settings.ini"),
	fileTheme(directory + "theme.ini"),
	fileStatistics(directory + "statistics.bin")
{
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

	if (! Utils::File::isDirectory(this->directory))
		Utils::File::mkdir_p(this->directory);

	if (! Utils::File::exists(this->fileSettings))
		Utils::File::create(this->fileSettings);

	if (! Utils::File::exists(this->fileTheme))
		Utils::File::create(this->fileTheme);

	if (! Utils::File::exists(this->fileStatistics))
		Utils::File::create(this->fileStatistics);

	// Now that the directories exist, let's make sure
	// it's on the global list of existing Profiles
	if (std::find(Profile::profiles.begin(), Profile::profiles.end(), name) == Profile::profiles.end())
	{
		Profile::profiles.push_back(name);
		std::sort(Profile::profiles.begin(), Profile::profiles.end());
	}

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
	settings.theme.textbox = Colors::pair(COLOR_WHITE, COLOR_BLUE);

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
void Profile::loadSettings()
{
	INI ini;
	if (! ini.load(this->fileSettings))
		return;

// Small macro to avoid unnecessary typing.
//
// To get something from the ini file we send the
// text (to identify some value) and the default
// value in case it doesn't exist.
//
// For the last one I send the variable itself,
// so we fallback to the default values.
#define INI_GET(var, text) \
	{ \
		var = ini.get(text, var); \
	}

	INI_GET(settings.screen.center_horizontally, "screen:center_horizontal");
	INI_GET(settings.screen.center_vertically,   "screen:center_vertical");

	INI_GET(settings.screen.show_borders,  "screen:borders");
	INI_GET(settings.screen.fancy_borders, "screen:fancy_borders");
	INI_GET(settings.screen.outer_border,  "screen:outer_border");

	INI_GET(settings.screen.use_colors, "screen:colors");

	INI_GET(settings.screen.show_statistics, "screen:statistics");

	// Game

	INI_GET(settings.game.next_pieces,    "game:next_pieces");
	INI_GET(settings.game.initial_noise,  "game:initial_noise");
	INI_GET(settings.game.starting_level, "game:starting_level");

	INI_GET(settings.game.has_ghost, "game:ghost");
	INI_GET(settings.game.can_hold,  "game:hold");

	INI_GET(settings.game.random_algorithm, "game:random_algorithm");

	INI_GET(settings.game.has_game_over_animation, "game:game_over_animation");
	INI_GET(settings.game.line_clear_delay,        "game:line_clear_delay");

	INI_GET(settings.game.slide_left,  "game:slide_left");
	INI_GET(settings.game.slide_right, "game:slide_right");
	INI_GET(settings.game.invisible,   "game:invisible");

	// Input
	// TODO: Validate keys

	// INT_GET(settings.input.left,  "input:left");
	// INI_GET(settings.input.right, "input:right");
	// INI_GET(settings.input.up,    "input:up");
	// INI_GET(settings.input.down,  "input:down");
	// INI_GET(settings.input.drop,  "input:drop");
	// INI_GET(settings.input.rotate_clockwise,        "input:rotate_clockwise");
	// INI_GET(settings.input.rotate_counterclockwise, "input:rotate_counterclockwise");
	// INI_GET(settings.input.pause,             "input:pause");
	// INI_GET(settings.input.hold,              "input:hold");
	// INI_GET(settings.input.toggle_statistics, "input:toggle_statistics");
	// INI_GET(settings.input.help,              "input:help");
	// INI_GET(settings.input.high_scores,       "input:high_scores");
	// INI_GET(settings.input.quit,              "input:quit");

	// Now, to the Theme file!

	ini.free();
	if (! ini.load(this->fileTheme))
		return;

	// FIXME: For now we're not dealing with colors,
	//        only with block appearances!

	INI_GET(settings.theme.piece_has_colors, "theme:piece_colors");
	INI_GET(settings.theme.ghost_has_colors, "theme:ghost_colors");
	INI_GET(settings.theme.show_pivot_block, "theme:show_pivot_block");
	INI_GET(settings.theme.lock_piece_color, "theme:lock_piece_colors");

// 	std::string tmp;

// #define INI_GET_THEME(var, text) \
// 	{ \
// 		tmp = ini.get(text, var.appearance); \
// 		var.appearance[0] = tmp[0]; \
// 		var.appearance[1] = tmp[1]; \
// 	}

// 	INI_GET_THEME(settings.theme.clear_line, "clear_line:block");

// 	INI_GET_THEME(settings.theme.piece_colorless, "piece_colorless:block");

// 	INI_GET_THEME(settings.theme.piece,     "piece:block");
// 	INI_GET_THEME(settings.theme.ghost,     "ghost:block");
// 	INI_GET_THEME(settings.theme.locked,    "locked:block");
// 	INI_GET_THEME(settings.theme.invisible, "invisible:block");
// 	INI_GET_THEME(settings.theme.piece_S,   "piece_S:block");
// 	INI_GET_THEME(settings.theme.piece_Z,   "piece_Z:block");
// 	INI_GET_THEME(settings.theme.piece_O,   "piece_O:block");
// 	INI_GET_THEME(settings.theme.piece_I,   "piece_I:block");
// 	INI_GET_THEME(settings.theme.piece_L,   "piece_L:block");
// 	INI_GET_THEME(settings.theme.piece_J,   "piece_J:block");
// 	INI_GET_THEME(settings.theme.piece_T,   "piece_T:block");
}
void Profile::saveSettings()
{
	INI ini;
	if (! ini.load(this->fileSettings))
		return;

#define INI_SET(text, var) \
	{ \
		ini.set(text, Utils::String::toString(var)); \
	}

	INI_SET("screen:center_horizontal", settings.screen.center_horizontally);
	INI_SET("screen:center_vertical",   settings.screen.center_vertically);

	INI_SET("screen:borders",       settings.screen.show_borders);
	INI_SET("screen:fancy_borders", settings.screen.fancy_borders);
	INI_SET("screen:outer_border",  settings.screen.outer_border);

	INI_SET("screen:colors", settings.screen.use_colors);

	INI_SET("screen:statistics", settings.screen.show_statistics);

	// Game

	INI_SET("game:next_pieces",    settings.game.next_pieces);
	INI_SET("game:initial_noise",  settings.game.initial_noise);
	INI_SET("game:starting_level", settings.game.starting_level);

	INI_SET("game:ghost", settings.game.has_ghost);
	INI_SET("game:hold",  settings.game.can_hold);

	INI_SET("game:random_algorithm", settings.game.random_algorithm);

	INI_SET("game:game_over_animation", settings.game.has_game_over_animation);
	INI_SET("game:line_clear_delay",    settings.game.line_clear_delay);

	INI_SET("game:slide_left",  settings.game.slide_left);
	INI_SET("game:slide_right", settings.game.slide_right);
	INI_SET("game:invisible",   settings.game.invisible);

	// Input
	// TODO: Validate keys

	// INT_GET(settings.input.left,  "input:left");
	// INI_SET(settings.input.right, "input:right");
	// INI_SET(settings.input.up,    "input:up");
	// INI_SET(settings.input.down,  "input:down");
	// INI_SET(settings.input.drop,  "input:drop");
	// INI_SET(settings.input.rotate_clockwise,        "input:rotate_clockwise");
	// INI_SET(settings.input.rotate_counterclockwise, "input:rotate_counterclockwise");
	// INI_SET(settings.input.pause,             "input:pause");
	// INI_SET(settings.input.hold,              "input:hold");
	// INI_SET(settings.input.toggle_statistics, "input:toggle_statistics");
	// INI_SET(settings.input.help,              "input:help");
	// INI_SET(settings.input.high_scores,       "input:high_scores");
	// INI_SET(settings.input.quit,              "input:quit");

	ini.save(this->fileSettings);

	// Now, to the Theme file!

	ini.free();
	if (! ini.load(this->fileTheme))
		return;

	// FIXME: For now we're not dealing with colors,
	//        only with block appearances!

	INI_SET("theme:piece_colors", settings.theme.piece_has_colors);
	INI_SET("theme:ghost_colors", settings.theme.ghost_has_colors);
	INI_SET("theme:show_pivot_block", settings.theme.show_pivot_block);
	INI_SET("theme:lock_piece_colors", settings.theme.lock_piece_color);

	ini.save(this->fileTheme);

// 	std::string tmp;

// #define INI_SET_THEME(var, text) \
// 	{ \
// 		tmp = ini.get(text, var.appearance); \
// 		var.appearance[0] = tmp[0]; \
// 		var.appearance[1] = tmp[1]; \
// 	}

// 	INI_SET_THEME(settings.theme.clear_line, "clear_line:block");

// 	INI_SET_THEME(settings.theme.piece_colorless, "piece_colorless:block");

// 	INI_SET_THEME(settings.theme.piece,     "piece:block");
// 	INI_SET_THEME(settings.theme.ghost,     "ghost:block");
// 	INI_SET_THEME(settings.theme.locked,    "locked:block");
// 	INI_SET_THEME(settings.theme.invisible, "invisible:block");
// 	INI_SET_THEME(settings.theme.piece_S,   "piece_S:block");
// 	INI_SET_THEME(settings.theme.piece_Z,   "piece_Z:block");
// 	INI_SET_THEME(settings.theme.piece_O,   "piece_O:block");
// 	INI_SET_THEME(settings.theme.piece_I,   "piece_I:block");
// 	INI_SET_THEME(settings.theme.piece_L,   "piece_L:block");
// 	INI_SET_THEME(settings.theme.piece_J,   "piece_J:block");
// 	INI_SET_THEME(settings.theme.piece_T,   "piece_T:block");
}


#include <Game/Entities/Profile.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/String.hpp>
#include <Engine/Helpers/File.hpp>
#include <Game/Config/Globals.hpp>
#include <Engine/Helpers/INI.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Engine/InputManager.hpp>

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
			if (Utils::String::back(files[i]) == '/')
				Utils::String::pop_back(&(files[i]));
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
	// C++11 compatibility
	// (I wish I could use `auto`)
	std::vector<std::string>::iterator where = std::find(Profile::profiles.begin(), Profile::profiles.end(), name);

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
		INI::Parser ini;
		ini.create();

		ini.top().addGroup("profiles");
		ini("profiles").addKey("default", name);

		try
		{
			ini.saveAs(Globals::Config::file);
		}
		catch(std::runtime_error& e)
		{
			// Couldn't save the file...
			// ... do nothing
		}
	}
	else
	{
		INI::Parser ini(Globals::Config::file);

		std::string buffer = ini("profiles")["default"];
		if (buffer.empty())
		{
			ini.top().addGroup("profiles");
			ini("profiles").addKey("default", name);

			try
			{
				ini.saveAs(Globals::Config::file);
			}
			catch(std::runtime_error& e)
			{
				// Couldn't save the file...
				// ... do nothing
			}
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

	settings.screen.animation_menu = std::string("random");
	settings.screen.animation_game = std::string("none");

	// Game
	settings.game.next_pieces = 7;

	settings.game.has_ghost = true;
	settings.game.can_hold  = true;

	settings.game.has_game_over_animation = true;
	settings.game.line_clear_delay = 200; // ms

	// Theme

	settings.theme.text        = ColorPair();
	settings.theme.hilite_text = Colors::pair("cyan", "default");
	settings.theme.textbox     = settings.theme.hilite_text.reverse();

	settings.theme.piece_has_colors = true;
	settings.theme.ghost_has_colors = true;
	settings.theme.show_pivot_block = false;
	settings.theme.lock_piece_color = false;

	settings.theme.clear_line = new Block(Colors::pair("white", "default"),
	                ':', ':');

	settings.theme.piece_colorless = new Block(Colors::pair("black", "white"),
	                ' ', ' ');

	settings.theme.piece = new Block(Colors::pair("black", "white"),
	                ' ', ' ');

	settings.theme.ghost = new Block(Colors::pair("black", "white", true),
	                '[', ']');

	settings.theme.locked = new Block(Colors::pair("white", "white"),
	                ' ', ' ');

	settings.theme.invisible = new Block(Colors::pair("white", "default"),
	                                      ' ', ' ');

	settings.theme.piece_S = new Block(Colors::pair("white", "green"),
	                ' ', ' ');

	settings.theme.piece_Z = new Block(Colors::pair("white", "red"),
	                ' ', ' ');

	settings.theme.piece_O = new Block(Colors::pair("white", "yellow"),
	                ' ', ' ');

	settings.theme.piece_I = new Block(Colors::pair("white", "cyan", true),
	                ' ', ' ');

	settings.theme.piece_L = new Block(Colors::pair("white", "yellow", true),
	                ' ', ' ');

	settings.theme.piece_J = new Block(Colors::pair("white", "blue"),
	                ' ', ' ');

	settings.theme.piece_T = new Block(Colors::pair("white", "magenta"),
	                ' ', ' ');

	this->resetKeybindings();

	this->scores = new ScoreFile();
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
void Profile::resetKeybindings()
{
	// Input
	// Loading default keys
	settings.input.left                    = KEY_LEFT;
	settings.input.right                   = KEY_RIGHT;
	settings.input.up                      = KEY_UP;
	settings.input.down                    = KEY_DOWN;
	settings.input.drop                    = ' ';
	settings.input.rotate_clockwise        = 'x';
	settings.input.rotate_counterclockwise = 'z';
	settings.input.rotate_180              = 'Z';
	settings.input.pause                   = 'p';
	settings.input.hold                    = 'c';
	settings.input.toggle_statistics       = '2';
	settings.input.help                    = 'h';
	settings.input.high_scores             = '1';
	settings.input.quit                    = 'q';

	// Now, binding them
	// Default Input configurations
	InputManager::bind("left",                    settings.input.left);
	InputManager::bind("right",                   settings.input.right);
//	InputManager::bind("up",                      settings.input.up);
	InputManager::bind("down",                    settings.input.down);
	InputManager::bind("drop",                    settings.input.drop);
	InputManager::bind("rotate_clockwise",        settings.input.rotate_clockwise);
	InputManager::bind("rotate_counterclockwise", settings.input.rotate_counterclockwise);
	InputManager::bind("rotate_180",              settings.input.rotate_180);
	InputManager::bind("pause",                   settings.input.pause);
	InputManager::bind("hold",                    settings.input.hold);
//	InputManager::bind("toggle_statistics",       settings.input.toggle_statistics);
	InputManager::bind("help",                    settings.input.help);
//	InputManager::bind("high_scores",             settings.input.high_scores);
	InputManager::bind("quit",                    settings.input.quit);
}
void Profile::loadSettings()
{
	INI::Parser* ini = NULL;

	try {
		ini = new INI::Parser(this->fileSettings);
	}
	catch(std::runtime_error& e)
	{
		// File doesn't exist (or we couldn't access it)
		// Either way, ignore it silently
		SAFE_DELETE(ini);
		return;
	}

	std::string buffer;

// Small macro to avoid unnecessary typing.
//
// To get something from the ini file we send the
// text (to identify some value) and the default
// value in case it doesn't exist.
//
// For the last one I send the variable itself,
// so we fallback to the default values.
#define INI_GET(var, out, in)                    \
	{                                            \
		buffer = (* ini)(out)[in];               \
		if (! buffer.empty())                    \
		{                                        \
			Utils::String::convert(buffer, var); \
		}                                        \
	}

	INI_GET(settings.screen.center_horizontally, "screen", "center_horizontal");
	INI_GET(settings.screen.center_vertically,   "screen", "center_vertical");

	INI_GET(settings.screen.show_borders,  "screen", "borders");
	INI_GET(settings.screen.fancy_borders, "screen", "fancy_borders");
	INI_GET(settings.screen.outer_border,  "screen", "outer_border");

	INI_GET(settings.screen.use_colors, "screen", "colors");

	INI_GET(settings.screen.show_statistics, "screen", "statistics");

	// Special case, getting a string
	INI_GET(settings.screen.animation_menu, "screen", "animation_menu");
	INI_GET(settings.screen.animation_game, "screen", "animation_game");

	// Game
	INI_GET(settings.game.next_pieces, "game", "next_pieces");
	INI_GET(settings.game.has_ghost,   "game", "ghost");
	INI_GET(settings.game.can_hold,    "game", "hold");

	INI_GET(settings.game.has_game_over_animation, "game", "game_over_animation");
	INI_GET(settings.game.line_clear_delay,        "game", "line_clear_delay");

	// Getting input keys
	std::string tmp;

	INI_GET(tmp, "input", "left");
	InputManager::bind("left", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "right");
	InputManager::bind("right", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "down");
	InputManager::bind("down", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "drop");
	InputManager::bind("drop", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "rotate_clockwise");
	InputManager::bind("rotate_clockwise", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "rotate_counterclockwise");
	InputManager::bind("rotate_counterclockwise", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "rotate_180");
	InputManager::bind("rotate_180", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "pause");
	InputManager::bind("pause", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "help");
	InputManager::bind("help", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "hold");
	InputManager::bind("hold", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "quit");
	InputManager::bind("quit", InputManager::stringToKey(tmp));

	// Now, to the Theme file!
	SAFE_DELETE(ini);

	try {
		ini = new INI::Parser(this->fileTheme);
	}
	catch(std::runtime_error& e)
	{
		// File doesn't exist (or we couldn't access it)
		// Either way, ignore it silently
		SAFE_DELETE(ini);
		return;
	}

	// FIXME: For now we're not dealing with colors,
	//        only with block appearances!

	INI_GET(settings.theme.piece_has_colors, "theme", "piece_colors");
	INI_GET(settings.theme.ghost_has_colors, "theme", "ghost_colors");
	INI_GET(settings.theme.show_pivot_block, "theme", "show_pivot_block");
	INI_GET(settings.theme.lock_piece_color, "theme", "lock_piece_colors");

// 	std::string tmp;

// #define INI_GET_THEME(var, text)
// 	{
// 		tmp = ini.get(text, var.appearance);
// 		var.appearance[0] = tmp[0];
// 		var.appearance[1] = tmp[1];
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

	SAFE_DELETE(ini);
}
void Profile::saveSettings()
{
	// Even if the file doesn't exist, we'll create it.
	INI::Parser* ini;

	try
	{
		ini = new INI::Parser(this->fileSettings);
	}
	catch(std::runtime_error& e)
	{
		ini = new INI::Parser();
		ini->create();
	}

	std::string buffer;

// Other macro to avoid typing, similar to the one
// at loadFile()
#define INI_SET(out, in, var)	               \
	{                                          \
		buffer = Utils::String::toString(var); \
		ini->top().addGroup(out);              \
		(* ini)(out).addKey(in, buffer);       \
	}

	INI_SET("screen", "center_horizontal", settings.screen.center_horizontally);
	INI_SET("screen", "center_vertical",   settings.screen.center_vertically);

	INI_SET("screen", "borders",       settings.screen.show_borders);
	INI_SET("screen", "fancy_borders", settings.screen.fancy_borders);
	INI_SET("screen", "outer_border",  settings.screen.outer_border);

	INI_SET("screen", "colors", settings.screen.use_colors);

	INI_SET("screen", "statistics", settings.screen.show_statistics);

	INI_SET("screen", "animation_menu",  settings.screen.animation_menu);
	INI_SET("screen", "animation_game",  settings.screen.animation_game);

	// Game

	INI_SET("game", "next_pieces", settings.game.next_pieces);
	INI_SET("game", "ghost",       settings.game.has_ghost);
	INI_SET("game", "hold",        settings.game.can_hold);

	INI_SET("game", "game_over_animation", settings.game.has_game_over_animation);
	INI_SET("game", "line_clear_delay",    settings.game.line_clear_delay);

	// Input Keys
	std::string key;

	key = InputManager::keyToString(InputManager::getBind("left"));
	INI_SET("input", "left", key);

	key = InputManager::keyToString(InputManager::getBind("right"));
	INI_SET("input", "right", key);

	key = InputManager::keyToString(InputManager::getBind("down"));
	INI_SET("input", "down", key);

	key = InputManager::keyToString(InputManager::getBind("drop"));
	INI_SET("input", "drop", key);

	key = InputManager::keyToString(InputManager::getBind("rotate_clockwise"));
	INI_SET("input", "rotate_clockwise", key);

	key = InputManager::keyToString(InputManager::getBind("rotate_counterclockwise"));
	INI_SET("input", "rotate_counterclockwise", key);

	key = InputManager::keyToString(InputManager::getBind("rotate_180"));
	INI_SET("input", "rotate_180", key);

	key = InputManager::keyToString(InputManager::getBind("pause"));
	INI_SET("input", "pause", key);

	key = InputManager::keyToString(InputManager::getBind("help"));
	INI_SET("input", "help", key);

	key = InputManager::keyToString(InputManager::getBind("hold"));
	INI_SET("input", "hold", key);

	key = InputManager::keyToString(InputManager::getBind("quit"));
	INI_SET("input", "quit", key);

	try
	{
		ini->saveAs(this->fileSettings);
	}
	catch(std::runtime_error& e)
	{
		// Couldn't save the file...
		// ... do nothing
	}

	// Now, to the Theme file!
	SAFE_DELETE(ini);

	try {
		ini = new INI::Parser(this->fileTheme);
	}
	catch(std::runtime_error& e)
	{
		ini = new INI::Parser();
		ini->create();
	}

	// FIXME: For now we're not dealing with colors,
	//        only with block appearances!

	INI_SET("theme", "piece_colors", settings.theme.piece_has_colors);
	INI_SET("theme", "ghost_colors", settings.theme.ghost_has_colors);
	INI_SET("theme", "show_pivot_block", settings.theme.show_pivot_block);
	INI_SET("theme", "lock_piece_colors", settings.theme.lock_piece_color);

	try
	{
		ini->saveAs(this->fileTheme);
	}
	catch(std::runtime_error& e)
	{
		// Couldn't save the file...
		// ... do nothing
	}

// 	std::string tmp;

// #define INI_SET_THEME(var, text)
// 	{
// 		tmp = ini.get(text, var.appearance);
// 		var.appearance[0] = tmp[0];
// 		var.appearance[1] = tmp[1];
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
	SAFE_DELETE(ini);
}

void Profile::applyGraphicalSettings()
{
	EngineGlobals::Screen::center_horizontally = this->settings.screen.center_horizontally;
	EngineGlobals::Screen::center_vertically   = this->settings.screen.center_vertically;
	EngineGlobals::Screen::show_borders        = this->settings.screen.show_borders;
	EngineGlobals::Screen::fancy_borders       = this->settings.screen.fancy_borders;
	EngineGlobals::Screen::outer_border        = this->settings.screen.outer_border;
}


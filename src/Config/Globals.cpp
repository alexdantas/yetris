#include <Config/Globals.hpp>
#include <Config/INI.hpp>
#include <Game/Block.hpp>
#include <Misc/Utils.hpp>

#include <ncurses.h>

// VERSION is formatted like "0.0.1" - i'm skipping the dots
int Globals::version[3] = { VERSION[0] - '0',
                            VERSION[2] - '0',
                            VERSION[4] - '0'};

//  ___   ___   ___   ____  _   _     ____  __
// | |_) | |_) / / \ | |_  | | | |   | |_  ( (`
// |_|   |_| \ \_\_/ |_|   |_| |_|__ |_|__ _)_)

Profile* Globals::Profiles::current = NULL;
std::string Globals::Profiles::default_name = "";

//  __   __    ___   ____  ____  _
// ( (` / /`  | |_) | |_  | |_  | |\ |
// _)_) \_\_, |_| \ |_|__ |_|__ |_| \|

bool Globals::Screen::center_horizontally = true;
bool Globals::Screen::center_vertically   = true;

bool Globals::Screen::show_borders  = true;
bool Globals::Screen::fancy_borders = true;
bool Globals::Screen::outer_border  = true;

bool Globals::Screen::use_colors = true;

bool Globals::Screen::show_statistics = true;

//  __     __    _      ____
// / /`_  / /\  | |\/| | |_
// \_\_/ /_/--\ |_|  | |_|__

int Globals::Game::next_pieces = 7;
int Globals::Game::initial_noise = 0;
unsigned int Globals::Game::starting_level = 1;

bool Globals::Game::has_ghost = true;
bool Globals::Game::can_hold  = true;

short Globals::Game::random_algorithm = 1;

bool Globals::Game::has_game_over_animation = true;
int  Globals::Game::line_clear_delay = 200; // ms

bool Globals::Game::slide_left  = false;
bool Globals::Game::slide_right = false;
bool Globals::Game::invisible = false;
// _____  _     ____  _      ____
//  | |  | |_| | |_  | |\/| | |_
//  |_|  |_| | |_|__ |_|  | |_|__

ColorPair Globals::Theme::text = Colors::pair(COLOR_WHITE, COLOR_DEFAULT);
ColorPair Globals::Theme::hilite_text = Colors::pair(COLOR_BLUE, COLOR_DEFAULT);

bool Globals::Theme::piece_has_colors = true;
bool Globals::Theme::ghost_has_colors = true;
bool Globals::Theme::show_pivot_block = false;
bool Globals::Theme::lock_piece_color = false;

Block* Globals::Theme::clear_line      = NULL;
Block* Globals::Theme::piece_colorless = NULL;
Block* Globals::Theme::piece           = NULL;
Block* Globals::Theme::ghost           = NULL;
Block* Globals::Theme::locked          = NULL;
Block* Globals::Theme::invisible       = NULL;

Block* Globals::Theme::piece_S = NULL;
Block* Globals::Theme::piece_Z = NULL;
Block* Globals::Theme::piece_O = NULL;
Block* Globals::Theme::piece_I = NULL;
Block* Globals::Theme::piece_L = NULL;
Block* Globals::Theme::piece_J = NULL;
Block* Globals::Theme::piece_T = NULL;

//  __    ___   _      ____  _   __
// / /`  / / \ | |\ | | |_  | | / /`_
// \_\_, \_\_/ |_| \| |_|   |_| \_\_/

// real initialization below
std::string Globals::Config::directory = "";

//  _   _      ___   _    _____
// | | | |\ | | |_) | | |  | |
// |_| |_| \| |_|   \_\_/  |_|

int Globals::Input::left                    = KEY_LEFT;
int Globals::Input::right                   = KEY_RIGHT;
int Globals::Input::up                      = KEY_UP;
int Globals::Input::down                    = KEY_DOWN;
int Globals::Input::drop                    = ' ';
int Globals::Input::rotate_clockwise        = 'x';
int Globals::Input::rotate_counterclockwise = 'z';
int Globals::Input::pause                   = 'p';
int Globals::Input::hold                    = 'c';
int Globals::Input::toggle_statistics       = '2';
int Globals::Input::help                    = 'h';
int Globals::Input::high_scores             = '1';
int Globals::Input::quit                    = 'q';

//  _   _      _  _____
// | | | |\ | | |  | |
// |_| |_| \| |_|  |_|

void Globals::init()
{
	Globals::Theme::clear_line = new Block(Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
	                ':', ':');

	Globals::Theme::piece_colorless = new Block(Colors::pair(COLOR_BLACK, COLOR_WHITE),
	                ' ', ' ');

	Globals::Theme::piece = new Block(Colors::pair(COLOR_BLACK, COLOR_WHITE),
	                ' ', ' ');

	Globals::Theme::ghost = new Block(Colors::pair(COLOR_BLACK, COLOR_WHITE, true),
	                '[', ']');

	Globals::Theme::locked = new Block(Colors::pair(COLOR_WHITE, COLOR_WHITE),
	                ' ', ' ');

	Globals::Theme::invisible = new Block(Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
	                                      ' ', ' ');

	Globals::Theme::piece_S = new Block(Colors::pair(COLOR_WHITE, COLOR_GREEN),
	                ' ', ' ');

	Globals::Theme::piece_Z = new Block(Colors::pair(COLOR_WHITE, COLOR_RED),
	                ' ', ' ');

	Globals::Theme::piece_O = new Block(Colors::pair(COLOR_WHITE, COLOR_YELLOW),
	                ' ', ' ');

	Globals::Theme::piece_I = new Block(Colors::pair(COLOR_WHITE, COLOR_CYAN, true),
	                ' ', ' ');

	Globals::Theme::piece_L = new Block(Colors::pair(COLOR_WHITE, COLOR_YELLOW, true),
	                ' ', ' ');

	Globals::Theme::piece_J = new Block(Colors::pair(COLOR_WHITE, COLOR_BLUE),
	                ' ', ' ');

	Globals::Theme::piece_T = new Block(Colors::pair(COLOR_WHITE, COLOR_MAGENTA),
	                ' ', ' ');

	// Making sure default config directory exists
	Globals::Config::directory = (Utils::File::getHome() +
	                              ".local/share/" +
	                              PACKAGE + "/");

	if (! Utils::File::isDirectory(Globals::Config::directory))
		Utils::File::mkdir_p(Globals::Config::directory);

	// Getting default profile name - should be at a
	// file 'global-settings.ini'.
	if (Utils::File::exists(Globals::Config::directory + "global-settings.ini"))
	{
		INI ini;
		if (ini.load(Globals::Config::directory + "global-settings.ini"))
		{
			Globals::Profiles::default_name = ini.get("profiles:default", Globals::Profiles::default_name);
		}
	}
}


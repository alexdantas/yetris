#include "Globals.hpp"
#include "Block.hpp"

int Globals::version[3] = { 0, 0, 1 };

//  __   __    ___   ____  ____  _
// ( (` / /`  | |_) | |_  | |_  | |\ |
// _)_) \_\_, |_| \ |_|__ |_|__ |_| \|

bool Globals::Screen::center_horizontally = true;
bool Globals::Screen::center_vertically   = true;

bool Globals::Screen::show_borders  = true;
bool Globals::Screen::fancy_borders = true;
bool Globals::Screen::outer_border  = true;

bool Globals::Screen::use_colors = true;

//  __     __    _      ____
// / /`_  / /\  | |\/| | |_
// \_\_/ /_/--\ |_|  | |_|__

int Globals::Game::next_pieces   = 5;
int Globals::Game::initial_noise = 0;

bool Globals::Game::has_ghost = true;
bool Globals::Game::can_hold  = true;

bool Globals::Game::has_statistics      = true;
bool Globals::Game::has_line_statistics = true;

short Globals::Game::random_algorithm = 0;

bool Globals::Game::has_game_over_animation = true;
bool Globals::Game::line_clear_timeout      = 20000; // microseconds

bool Globals::Game::slide_left  = false;
bool Globals::Game::slide_right = false;

// _____  _     ____  _      ____
//  | |  | |_| | |_  | |\/| | |_
//  |_|  |_| | |_|__ |_|  | |_|__

bool Globals::Theme::piece_has_colors = true;
bool Globals::Theme::ghost_has_colors = true;
bool Globals::Theme::show_pivot_block = false;
bool Globals::Theme::lock_piece_color = false;

Block* Globals::Theme::clear_line      = NULL;
Block* Globals::Theme::piece_colorless = NULL;
Block* Globals::Theme::piece           = NULL;
Block* Globals::Theme::ghost           = NULL;
Block* Globals::Theme::locked          = NULL;

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

std::string Globals::Config::config_filename = "";
std::string Globals::Config::hscore_filename = "";



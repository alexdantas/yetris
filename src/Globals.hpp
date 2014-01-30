#ifndef GLOBALS_H_DEFINED
#define GLOBALS_H_DEFINED

#include "Block.hpp"

/// All global settings to the game.
///
namespace Globals
{
	/// Game version (format MMP - Major Minor Patch)
	extern int version[3];

	namespace Screen
	{
		extern bool center_horizontally;
		extern bool center_vertically;

		extern bool show_borders;
		extern bool fancy_borders;
		extern bool outer_border;

		extern bool use_colors;
	};
	namespace Game
	{
		extern int next_pieces;
		extern int initial_noise;

		extern bool has_ghost;
		extern bool can_hold;

		extern bool has_statistics;
		extern bool has_line_statistics;

		extern short random_algorithm;

		extern bool has_game_over_animation;
		extern bool line_clear_timeout;
		extern bool slide_left;
		extern bool slide_right;
	};
	namespace Theme
	{
		extern bool piece_has_colors;
		extern bool ghost_has_colors;
		extern bool show_pivot_block;
		extern bool lock_piece_color;

		extern Block* clear_line;
		extern Block* piece_colorless;
		extern Block* piece;
		extern Block* ghost;
		extern Block* locked;
		extern Block* piece_S;
		extern Block* piece_Z;
		extern Block* piece_O;
		extern Block* piece_I;
		extern Block* piece_L;
		extern Block* piece_J;
		extern Block* piece_T;
	};

	namespace Config
	{
		extern std::string config_filename;
		extern std::string hscore_filename;
	};
};

#endif //GLOBALS_H_DEFINED


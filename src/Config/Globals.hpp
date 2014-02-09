#ifndef GLOBALS_H_DEFINED
#define GLOBALS_H_DEFINED

#include <Game/Block.hpp>
#include <Interface/Colors.hpp>

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

		extern bool show_statistics;
	};
	namespace Game
	{
		extern int next_pieces;
		extern int initial_noise;
		extern unsigned int starting_level;

		extern bool has_ghost;
		extern bool can_hold;

		extern short random_algorithm;

		extern bool has_game_over_animation;
		extern bool slide_left;
		extern bool slide_right;

		extern int line_clear_delay; // ms

		extern bool invisible;
	};
	namespace Theme
	{
		extern ColorPair text;
		extern ColorPair hilite_text;

		extern bool piece_has_colors;
		extern bool ghost_has_colors;
		extern bool show_pivot_block;
		extern bool lock_piece_color;

		extern Block* clear_line;
		extern Block* piece_colorless;
		extern Block* piece;
		extern Block* ghost;
		extern Block* locked;
		extern Block* invisible;

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
	namespace Input
	{
		extern int left;
		extern int right;
		extern int up;
		extern int down;
		extern int drop;
		extern int rotate_clockwise;
		extern int rotate_counterclockwise;
		extern int pause;
		extern int hold;
		extern int toggle_statistics;
		extern int help;
		extern int high_scores;
		extern int quit;
	};
};

#endif //GLOBALS_H_DEFINED


#ifndef PROFILE_H_DEFINED
#define PROFILE_H_DEFINED

#include <Game/Entities/Block.hpp>
#include <Game/Entities/ScoreFile.hpp>
#include <Engine/Graphics/Colors.hpp>

#include <vector>
#include <string>

/// Maximum size that each profile name may have.
#define MAX_NAME_SIZE 255

/// Represents an user of the game - with individual settings.
///
/// ## Rationale
///
/// Each profile has specific statistics, scores and settings.
///
/// That's because multiple people may want to play this game
/// on the same machine.
///
/// Even then, one may want to have multiple accounts to test
/// it's Tetris skills.
///
/// On the future I might implement Achievements, so it makes
/// sense to have Profiles.
///
class Profile
{
public:
	/// Loads all existing profiles.
	///
	/// @return If we could load profiles. If not, this is
	///         probably the first time playing.
	static bool load();

	/// Tells if #name is a valid one for creating a Profile.
	/// It checks for invalid characters.
	static bool isNameValid(std::string name);

	/// Deletes profile with #name.
	///
	/// @warning There's no going back! All settings, scores
	///          and statistics will be gone!
	static void remove(std::string name);

	/// Creates profile's files for #name.
	///
	/// @note It doesn't load it. This method creates it
	///       by assuring it's files exist.
	///       To load settings and stuff, create
	///       a new Profile class.
	///
	static void create(std::string name);

	/// Loads profile with #name.
	///
	/// @note If it doesn't exist, will create it.
	///       If it does exist, will load it's settings.
	///
	/// If you just want to create a profile without
	/// loading it, look towards Profile::create.
	Profile(std::string name);

	virtual ~Profile();

	/// The user name.
	/// Also defines the directory where to place config files.
	std::string name;

	/// Root directory to store things of this Profile.
	std::string directory;

	/// Main settings file name of this Profile.
	std::string fileSettings;

	/// Where we store piece's appearances and colors.
	std::string fileTheme;

	/// Where we store statistics of this Profile.
	std::string fileStatistics;

	ScoreFile* scores;

	void resetKeybindings();

	/// Loads settings based on the user configuration file.
	void loadSettings();

	/// Saves current settings on the user configuration file.
	void saveSettings();

	/// Contains all the settings for this individual Profile.
	///
	/// Look, I know it's ugly as fuark to call huge-ass
	/// variables like:
	///
	/// Globals::Profiles::current::Settings::Screen::use_colors
	///
	/// But I couldn't think of a better way to separate
	/// everything and keep things organized :(
	///
	/// If you do, please contact me ;_;
	///
	struct Settings
	{
		struct Screen
		{
			bool center_horizontally;
			bool center_vertically;

			bool show_borders;
			bool fancy_borders;
			bool outer_border;

			bool use_colors;

			bool show_statistics;
		};
		Screen screen;

		struct Game
		{
			int lines;

			int level;
			int starting_level;

			int initial_noise;

			bool invisible;
			bool slide_left;
			bool slide_right;

			std::string random_algorithm;

			// Information for appearance and stuff
			int next_pieces;
			bool has_ghost;
			bool can_hold;

			bool has_game_over_animation;

			int line_clear_delay; // ms
		};
		Game game;

		struct Theme
		{
			ColorPair text;
			ColorPair hilite_text;
			ColorPair textbox;

			bool piece_has_colors;
			bool ghost_has_colors;
			bool show_pivot_block;
			bool lock_piece_color;

			Block* clear_line;
			Block* piece_colorless;
			Block* piece;
			Block* ghost;
			Block* locked;
			Block* invisible;

			Block* piece_S;
			Block* piece_Z;
			Block* piece_O;
			Block* piece_I;
			Block* piece_L;
			Block* piece_J;
			Block* piece_T;
		};
		Theme theme;

		struct Input
		{
			int left;
			int right;
			int up;
			int down;
			int drop;
			int rotate_clockwise;
			int rotate_counterclockwise;
			int rotate_180;
			int pause;
			int hold;
			int toggle_statistics;
			int help;
			int high_scores;
			int quit;
		};
		Input input;
	};
	Settings settings;

	/// All profiles present on the time of execution.
	static std::vector<std::string> profiles;
};

#endif //PROFILE_H_DEFINED


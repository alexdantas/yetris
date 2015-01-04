#ifndef PROFILE_H_DEFINED
#define PROFILE_H_DEFINED

#include <Game/Entities/Block.hpp>
#include <Game/Entities/ScoreFile.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Engine/EngineGlobals.hpp>

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
/// ## Note for graphic settings
///
/// This Game Engine has some visual settings such as centering
/// the game screen or drawing borders on windows.
/// They're at "Engine/EngineGlobals.hpp".
///
/// What I do is keep a copy of those variables and, when you
/// make changes, I apply them with `Profile::applyGraphicalSettings()`.
/// So I never deal with the Engine variables directly, only
/// with the Profile ones!
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

	/// Apply the visual settings directly on the Game Engine.
	///
	/// After that you should restart the current Layout and you'll
	/// see the changes immediately.
	void applyGraphicalSettings();

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

			/// Which animation to show on the background of the main menu.
			///
			/// Options (so far):
			///     "random", "fire", "water", "snakes", "life" and "none"
			std::string animation_menu;

			/// Same as above, but during the game.
			std::string animation_game;
		};
		Screen screen;

		struct Game
		{
			/**
			 * How many next pieces appear onscreen.
			 */
			int next_pieces;

			/**
			 * Is there a Ghost piece on the game?
			 *
			 * @note Little indication of where the piece will land.
			 */
			bool has_ghost;

			/**
			 * Can the player hold pieces?
			 *
			 * @note Keep the current piece on a temporary area,
			 *       switching it with whatever other piece was
			 *       there already.
			 */
			bool can_hold;

			bool has_game_over_animation;

			/**
			 * Delay to wait when the player clears a line.
			 *
			 * @note In milliseconds.
			 */
			int line_clear_delay;
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


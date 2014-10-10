#ifndef SCORE_H_DEFINED
#define SCORE_H_DEFINED

#include <Game/Config/Globals.hpp>

#include <string>
#include <vector>
#include <exception>

/// Custom exception class to specify an error that
/// occurred during a level loading.
///
class ScoreFileException : public std::exception
{
public:
	ScoreFileException(std::string message):
		message(message)
	{ }
	~ScoreFileException() throw()
	{ }

	std::string message;
};

/// A single entry on the high-score file.
///
struct ScoreEntry
{
	int points;

	int lines;

	int level;
	int starting_level;

	int initial_noise;

	bool invisible;
	bool slide_left;
	bool slide_right;

	std::string random_algorithm;

	/// Creates an empty score entry.
	/// Since everything is public, access them
	/// without thinking twice.
	ScoreEntry();

	/// Tells if both scores were made on exact same game settings.
	///
	/// Scores are incompatible if they doesn't have the same
	/// game settings.
	/// It's unfair for a score with less fruits to be compared
	/// with one where more fruits were enabled.
	///
	bool isLike(ScoreEntry& other);
};

/// Stores points the player made on the game.
///
/// There's different high scores for different
/// game settings.
///
/// Each time the user runs the game with a different
/// configuration of the tweaks above, a new high
/// score is generated.
///
/// It always starts with 0 and if the player
/// surpasses it, will be the new maximum.
///
class ScoreFile
{
public:
	/// Default extension to save the score files.
	///
	/// It's all the part that comes _after the dot_ on
	/// a file name.
	///
	/// @note Defaults to "yetrisscores"
	static std::string extension;

	/// Erases all high score files.
	///
	/// By "all" I mean every single score file from the arcade
	/// mode to all levels.
	///
	static void eraseAll();

	/// Full path to the score filename.
	///
	/// Defaults to:
	///
	///     config_dir + profile_name + "hscores" + ScoreFile::extension
	///
	std::string file;

	/// Creates a new score handler for the level #levelName.
	///
	/// @note It doesn't actually work unless you call #load, mate
	ScoreFile();

	/// Loads all high score entries based on a level name.
	///
	/// The file where we save the high scores will be based
	/// on the level name.
	///
	/// @note If `levelName` is empty, will fall back to
	///       the default score file (see Globals).
	///
	void load();

	/// Saves all the current scores on the file.
	void save();

	/// Checks if #score is the highest score and
	/// make it so.
	///
	/// Call this when you finished the game.
	///
	/// @return If #score just became the highest score.
	bool handle(ScoreEntry* score);

	/// Current score for this game.
	ScoreEntry score;

	/// Maximum high score obtained for the current game.
	///
	/// It will point to an element within #entries that
	/// has the highest score for this game settings.
	///
	/// @note If this is NULL then we couldn't open the
	///       high score files!
	///
	ScoreEntry* highScore;

private:
	/// All score entries
	std::vector<ScoreEntry>	entries;
};

#endif //SCORE_H_DEFINED


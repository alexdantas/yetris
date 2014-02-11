#ifndef PROFILE_H_DEFINED
#define PROFILE_H_DEFINED

#include <string>

/// Maximum size that each profile name may have.
#define MAX_NAME_SIZE 255

/// Represents an user of the game.
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
	/// @return The default profile name or an empty string
	///         if there's no profiles yet (first time playing).
	static std::string load();

	Profile(std::string name);
	virtual ~Profile();

	std::string getName();

private:
	std::string name;
};

#endif //PROFILE_H_DEFINED


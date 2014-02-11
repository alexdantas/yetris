#include <Game/Profile.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

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
			dirs.push_back(files[i]);

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

Profile::Profile(std::string name):
	name(name)
{
	// Will load profile's files.
}
Profile::~Profile()
{

}
std::string Profile::getName()
{
	return this->name;
}


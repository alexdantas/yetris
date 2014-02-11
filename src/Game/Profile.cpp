#include <Game/Profile.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>
#include <Config/INI.hpp>

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
			if (files[i].back() == '/')
				files[i].pop_back();
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

Profile::Profile(std::string name):
	name(name)
{
	std::string root   = Globals::Config::directory + name + "/";
	std::string config = root + "settings.ini";
	std::string stats  = root + "statistics.bin";

	// Make sure we can load a default profile at startup
	// (meaning the global settings file must have a profile
	//  name at `profiles:default`)

	if (! Utils::File::exists(Globals::Config::file))
	{
		INI ini;
		ini.create();
		ini.set("profiles:default", name);
		ini.save(Globals::Config::file);
	}
	else
	{
		INI ini;
		ini.load(Globals::Config::file);
		std::string default_name = ini.get("profiles:default", "");

		if (default_name.empty())
		{
			ini.set("profiles:default", name);
			ini.save(Globals::Config::file);
		}
	}


	// Making sure default directories and files exist.

	if (! Utils::File::isDirectory(root))
		Utils::File::mkdir_p(root);

	if (! Utils::File::exists(config))
		Utils::File::create(config);

	if (! Utils::File::exists(stats))
		Utils::File::create(stats);

	// Now will actuall load the files.
}
Profile::~Profile()
{

}
std::string Profile::getName()
{
	return this->name;
}


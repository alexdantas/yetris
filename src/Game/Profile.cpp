#include <Game/Profile.hpp>

std::string Profile::load()
{
	// See if there's any profiles already on the system.

	// Woops, couldn't find any!
	// Is this your first time or is there an issue?
	return "";
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


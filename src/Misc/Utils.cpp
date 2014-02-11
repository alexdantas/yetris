//  _________________________________________________________
// |       _        __    ___   _      _   _      __         |
// |      \ \    / / /\  | |_) | |\ | | | | |\ | / /`_       |
// |       \_\/\/ /_/--\ |_| \ |_| \| |_| |_| \| \_\_/       |
// |                                                         |
// | Adventurers, beware...                                  |
// |                                                         |
// | * The following file has lots of methods accumulated    |
// |   over the years.                                       |
// | * There isn't too much cohesion between them, so try    |
// |   to understand them individually.                      |
// |                                                         |
// |                                                         |
// | * They're mostly poor-coded, sorry 'bout that.          |
// |                                                         |
// `---------------------------------------------------------'

#include <Misc/Utils.hpp>

#include <sstream>    // sstream
#include <algorithm>  // find_if
#include <utility>    // C++11
#include <random>     // C++11
#include <ctime>      // time()
#include <unistd.h>	  // usleep()

//  ___    __    _      ___   ___   _
// | |_)  / /\  | |\ | | | \ / / \ | |\/|
// |_| \ /_/--\ |_| \| |_|_/ \_\_/ |_|  |

namespace Utils
{
	namespace Random
	{
		std::mt19937 engine;
	};
};

void Utils::Random::seed()
{
	engine.seed(time(nullptr));
}

int Utils::Random::between(int min, int max)
{
	std::uniform_int_distribution<int> distance(min, max);
	return distance(engine);
}

float Utils::Random::fbetween(float min, float max)
{
	std::uniform_real_distribution<float> distance(min, max);
	return distance(engine);
}

bool Utils::Random::boolean()
{
	std::uniform_int_distribution<int> distance(0, 10);

	return ((distance(engine) % 2) == 0);
}

bool Utils::Random::booleanWithChance(float percent)
{
	int x = Utils::Random::between(0, 9999);

	return (x < (percent * 100));
}

//  ___  _____  _     ____  ___   __
// / / \  | |  | |_| | |_  | |_) ( (`
// \_\_/  |_|  |_| | |_|__ |_| \ _)_)

std::string Utils::intToString(int num)
{
	// C++11
	return std::to_string(num);
}

int Utils::stringToInt(std::string text)
{
	// C++11
	return std::stoi(text);
}

//  __  _____  ___   _   _      __
// ( (`  | |  | |_) | | | |\ | / /`_
// _)_)  |_|  |_| \ |_| |_| \| \_\_/

std::string& Utils::String::ltrim(std::string &str)
{
	// Using some std black magic. Taken from here:
	// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

	// Here we create a predicate to be compared.
	// (In other words, "an element that `isspace`")
	std::pointer_to_unary_function<int, int> function = std::ptr_fun<int, int>(std::isspace);

	// This returns the first element that's not a space.
	//
	// It will go inside `str` looking for the first
	// element that matches a predicate.

	std::string::iterator it = std::find_if(str.begin(),
	                                        str.end(),

	                                        // And here we negate the predicate
	                                        // ("an element that's NOT `isspace`")
	                                        std::not1(function));

	// And here we erase everything up to it.
	str.erase(str.begin(),
	          it);

	return str;
}

std::string& Utils::String::rtrim(std::string& str)
{
	// More std magic. Sorry for the mess.
	// Please check method above (`ltrim`).

	str.erase(std::find_if(str.rbegin(),
	                       str.rend(),
	                       std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
	          str.end());

	return str;
}

std::string& Utils::String::trim(std::string& str)
{
	return Utils::String::ltrim(Utils::String::rtrim(str));
}

std::vector<std::string> Utils::String::split(const std::string& str, char delim)
{
	std::stringstream ss(str);      // "buffer"
	std::string item;               // current thing
	std::vector<std::string> elems; // all things

	while (std::getline(ss, item, delim))
		elems.push_back(Utils::String::trim(item));

	return elems;
}

// _____  _   _      ____
//  | |  | | | |\/| | |_
//  |_|  |_| |_|  | |_|__

void Utils::Time::delay_ms(int delay)
{
	usleep((useconds_t)delay * 100);
}

//  ____  _   _     ____
// | |_  | | | |   | |_
// |_|   |_| |_|__ |_|__

bool Utils::File::exists(std::string path)
{
	return (Utils::File::size(path) != -1);
}
off_t Utils::File::size(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) < 0)
		return -1;

	return s.st_size;
}
void Utils::File::mkdir_p(std::string path)
{
	std::string tmp(path);

	if (tmp.back() == '/')
		tmp[tmp.size() - 1] = '\0';

	for (std::string::iterator p = tmp.begin();
	     (*p) != '\0';
	     p++)
	{
		if (*p == '/')
		{
			*p = '\0';
			mkdir(tmp.c_str(), S_IRWXU);
			*p = '/';
		}
	}
	mkdir(tmp.c_str(), S_IRWXU);
}


#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <string>
#include <vector>

/// Random things accumulated over the years.
///
///
namespace Utils
{
	/// Better random number generator.
	namespace Random
	{
		/// Must be called.
		void seed();

		/// Random number between `min` and `max`.
		int between(int min, int max);

		/// Random float number between `min` and `max`.
		float fbetween(float min, float max);

		/// Random boolean.
		bool boolean();
	};

	/// Converts `num` into a `std::string`.
	std::string intToString(int num);

	/// Converts `text` into an int`.
	int stringToInt(std::string text);

	namespace String
	{
		/// Removes all space on the left of `str`.
		std::string& ltrim(std::string &str);

		/// Removes all space on the right of `str`.
		std::string& rtrim(std::string& str);

		/// Removes all space on both sides of `str`.
		std::string& trim(std::string& str);

		/// Splits `str` according to `delimt`.
		///
		/// @return A vector with two elements - string
		///         before and string after `delimit`.
		///
		std::vector<std::string> split(const std::string& str, char delim);
	};
};

#endif //UTILS_H_DEFINED


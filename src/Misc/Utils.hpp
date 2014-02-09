#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <string>
#include <vector>
#include <unistd.h>

/// Random useful things accumulated over the years.
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

		/// Random boolean with chance of #percent.
		/// @note Precision up to 2 decimal digits.
		bool booleanWithChance(float percent);
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

	/// Stops execution for #delay microseconds.
	void delay_ms(int delay);
};

// Useful #defines collected over the years.

/// Deletes #pointer if valid, invalidating it after.
#define SAFE_DELETE(pointer) \
	{ \
		if (pointer) \
		{ \
			delete(pointer); \
			pointer = nullptr; \
		} \
	}

/// Tells the compiler we're not using #thing as an argument.
/// Don't forget ';' after it!
#define UNUSED(thing) \
	{ \
		(void)(thing); \
	  \
	}

#endif //UTILS_H_DEFINED


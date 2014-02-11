#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <string>
#include <vector>
#include <sys/stat.h> // mkdir() and off_t

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

	namespace Time
	{
		/// Stops execution for #delay microseconds.
		void delay_ms(int delay);
	};

	/// File I/O and Operational System's utilities.
	///
	/// Note, I'm using several POSIX functions.
	/// So the following functions surely aren't
	/// portable to Windows.
	/// Other systems are kinda unpredictable.
	///
	namespace File
	{
		/// Tells if #path exists.
		///
		/// @note It could be a file, directory or whatever.
		bool exists(std::string path);

		/// Returns the file size of #path in bytes.
		///
		/// @return It's size or -1 if it doesn't exist
		///         (or something strange happened).
		off_t size(std::string path);

		/// Creates #path directory hierarchy recursively,
		/// just like UNIX command `mkdir -p`.
		void mkdir_p(std::string path);

		/// Tells if #path is a directory.
		///
		/// @note Returns false also if something wrong happened.
		bool isDirectory(std::string path);

		/// Tells if #path is a regular file
		/// (not a directory, socket, FIFO device or whatever).
		///
		/// @note Returns false also if something wrong happened.
		bool isFile(std::string path);

		/// Lists all files withing #path.
		///
		/// @note The returned vecor is not ordered and
		///       all file names contain the full #path
		///       before them.
		std::vector<std::string> ls(std::string path);

		/// Gets the full path of the home directory for
		/// the user running this program.
		///
		/// @return The path or an empty string.
		/// @note We guarantee that the path has a trailing '/'.
		std::string getHome();

		/// Gets the user name of the person running this program.
		std::string getUser();
	};
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


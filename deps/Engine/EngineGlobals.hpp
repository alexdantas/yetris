#ifndef ENGINEGLOBALS_H_DEFINED
#define ENGINEGLOBALS_H_DEFINED

#include <Engine/Graphics/Colors.hpp>

/// Settings for the whole Game Engine.
/// They're mostly related to the appearance of the game.
///
/// As soon as you change these variables and refresh the Layout,
/// they're applied.
///
/// For example, if you want to center the screen at any
/// time of the game, do the following (inside a GameState):
///
///     EngineGlobals::Screen::center_horizontally = true;
///     this->layout->windowsExit();
///     this->layout->windowsInit();
///
/// What I usually do is create a "Profile" class with a copy
/// of those variables below. Then when the game starts I
/// apply them to thoes below and then it's ok!
///
namespace EngineGlobals
{
	/// Affects the way game Windows are drawn.
	/// Windows are basically segments of the screen, with a
	/// special one that contains every other, called the "main window".
	namespace Screen
	{
		/// Center the whole game screen on the X axis?
		extern bool center_horizontally;

		/// Center the whole game screen on the Y axis?
		extern bool center_vertically;

		/// If all game Windows have visible borders on their outside.
		extern bool show_borders;

		/// If the borders on the game Windows are drawn with extended
		/// ANSI characters or plain old ASCII characters.
		extern bool fancy_borders;

		/// If the main game Window (that has all the other Windows)
		/// has an external border.
		extern bool outer_border;
	};

	/// Affects the appearance of things drawn on menus and such.
	namespace Theme
	{
		/// Color for default, regular plain text.
		extern ColorPair text;

		/// Color for highlighted text.
		extern ColorPair hilite_text;

		/// Color for extra highlighted text;
		extern ColorPair hilite_hilite_text;

		/// Color for text a little bit hidden on the screen.
		extern ColorPair dim_text;

		/// Color for text very hard to see.
		extern ColorPair dim_dim_text;

		/// Color for textbox prompts (where you enter information).
		/// Usually it should be the reverse of the `hilite_text`.
		extern ColorPair textbox;
	};

	/// Starts the global engine variables with their default values.
	/// You can call this multiple times, it will always get reset.
	///
	/// @note This should be the first function you should call on
	///       your `main.cpp` - even before `Ncurses::init()`.
	///
	void init();
};

#endif //ENGINEGLOBALS_H_DEFINED


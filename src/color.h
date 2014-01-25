/* nFlood - Flood-it puzzle game on the terminal.
 * Copyright (C) 2011-2014  Alexandre Dantas <eu@alexdantas.net>
 * Copyright (C) 2014 Yu-Jie Lin <livibetter@gmail.com>
 *
 * nFlood is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://nflood.alexdantas.net
 */

#ifndef COLOR_H_DEFINED
#define COLOR_H_DEFINED

#include <ncurses.h>

/** This file handles all color schemes of nCurses.
 *
 *  You can't do nothing with a single `color_t`.
 *  Color pairs (`color_pair_t`) are what you're looking for.
 *
 *  You can set a color pair, so that next texts you write on
 *  the screen will have their appearance.
 *
 *  Single colors only exist to build color pairs.
 *  See the functions on this module and how they fit together
 *  to build color pairs from colors.
 *
 *  Starting off, you have the following colors:
 *
 *  `COLOR_BLACK`
 *  `COLOR_RED`
 *  `COLOR_GREEN`
 *  `COLOR_YELLOW`
 *  `COLOR_BLUE`
 *  `COLOR_MAGENTA`
 *  `COLOR_CYAN`
 *  `COLOR_WHITE`
 *  `COLOR_DEFAULT`  (default color of your terminal)
 *
 *  You can combine them as foreground and background to
 *  make color pairs.
 *  Also, you can create your own RGB colors with
 *  `color_rgb` and `color_hex`.
 *
 *  @note `COLORS` is nCurses' alias for the maximum number of
 *        currently supported colors.
 *        For old terminals that's 8 and for modern ones is 256.
 *
 */

typedef short  color_t;
typedef chtype color_pair_t;

#define COLOR_DEFAULT -1

/** Initializes color support and all color pairs.
 *  @return false (0) if something weird happened, else true (1).
 */
bool color_init();

/** Returns a random color within all basic color pairs.
 *
 *  @note Doesn't include default background
 */
color_t color_random();

/** Returns a color with red, green and blue components.
 *
 *  @note `r`, `g` and `b` accept values from 0 to 255.
 */
color_t color_rgb(short r, short g, short b);

/** Returns color with red, green and blue components in hex string.
 *
 *  @note The format accepted or `hex` is "#RRGGBB".
 *        RR, GG and BB are hex values to red, green and blue.
 *
 *  @bug If the format string is invalid, won't do anything.
 */
color_t color_hex(const char* hex);

/** Returns a color value from string `str`.
 *
 *  @note Currently accepted values are the most simple colors:
 *        "black", "red", "green", "yellow",
 *        "blue", "magenta", "cyan", "white" and
 *        "default" (for user terminal's default).
 *
 *  @note You'll probably use the returned value on
 *        `color_pair`.
 */
color_t color_from_string(const char* str);

/** Returns the color pair for `foreground` and `background`.
 *
 *  The value returned is only useful for `color_change_pair`.
 *
 *  @note `foreground` and `background` are internal vaues.
 *        To see them, check the comment at the beginning
 *        of this module.
 */
color_pair_t color_pair(color_t foreground, color_t background, bool is_bold);

/** Returns the color pair for strings `foreground`
 *  and `background`.
 *
 *  @note Currently accepted values are
 *        the same as `color_from_string`.
 */
color_pair_t color_pair_from_string(const char* foreground, const char* background);

/** Makes color pair active, so things we show on the screen
 *  from now on will have it's appearance.
 */
void color_activate(WINDOW* window, color_t foreground, color_t background);

/** Makes color pair active, so things we show on the screen
 *  from now on will have it's appearance.
 */
void color_pair_activate(WINDOW* window, color_pair_t pair);

#endif /* COLOR_H_DEFINED */


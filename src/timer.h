/* yetris - Tetris(tm) on the console.
 * Copyright (C) 2013 Alexandre Dantas (kure)
 *
 * This file is part of yetris.
 *
 * yetris is free software: you can redistribute it and/or modify
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
 * homepage: http://www.github.com/alexdantas/yetris/
 * mailto:   alex.dantas92@gmail.com
 */

/** @file timer.h
 *  Declaration of the timer structure and functions.
 *
 *  Start the timer by calling timer_start() and set its delay
 *  by giving it to timer_set_delay().
 *  The main use of this module is to compare the timer (through timer_delta())
 *  with the delay (returned by timer_get_delay()). Remember to call
 *  timer_stop() often!
 *
 *  gettimeofday() is Unix, GNU/Linux and Mac OS X system-specific.
 *  The only portable function is time.h's clock(), but it isn't very precise.
 *  <a href="http://www.songho.ca/misc/timer/timer.html"> Here's the source </a>.
 */

#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED

#include <sys/time.h>


/** The timer structure */
struct timert
{
	struct timeval start; /**< The timer start point */
	struct timeval end;   /**< The timer stop point */
};


long timer_delta_useconds (struct timert* t);
long timer_delta_mseconds(struct timert* t);
long timer_delta_seconds(struct timert* t);
long timer_delta_minutes(struct timert* t);
long timer_delta_hours(struct timert* t);
int  timer_start (struct timert* t);
int  timer_stop (struct timert* t);


#endif


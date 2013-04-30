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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	 If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://www.github.com/alexdantas/yetris/
 * mailto:	 alex.dantas92@gmail.com
 */

/** @file timer.h
 *	Defines stuff related to the timer.
 *
 *	Basically, it starts a timer, stops it and returns the delta between
 *	them in several time units.
 *
 *	@note If both timer_start() and timer_stop() don't get called
 *		  before any timer_delta_*() function, the results are unpredictable.
 *
 *	This module uses gettimeofday().
 *	It is Unix, GNU/Linux and Mac OS X system-specific.
 *	The only portable function is time.h's clock(), but it isn't very precise.
 *	See: http://www.songho.ca/misc/timer/timer.html
 */

#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED

#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h>

/** The timer structure */
typedef struct timer_s
{
	suseconds_t start_mark; /**< The timer start point */
	suseconds_t stop_mark;	/**< The timer stop point */
	suseconds_t pause_mark; /**< In case we pause the timer */
	bool running;
	bool paused;
} timer_s;

/** Starts the timer.
 *	@note If called multiple times, restarts the timer.
 */
void timer_start(timer_s* t);

/** Pauses the timer.
 *	@note If called multiple times, it does nothing.
 */
void timer_pause(timer_s* t);

/** Unpauses the timer.
 *	@note If the timer's not paused or this is called multiple times,
 *	it does nothing.
 */
void timer_unpause(timer_s* t);

/** Returns the time difference in microseconds (1/1000000 seconds). */
long timer_delta_us(timer_s* t);

 /** Returns the time difference in mili (1/1000 seconds). */
long timer_delta_ms(timer_s* t);

/** Returns the time difference in seconds. */
long timer_delta_s(timer_s* t);

/** Returns the time difference in minutes (60 seconds). */
long timer_delta_m(timer_s* t);

/** Returns the time difference in hours (3600 seconds). */
long timer_delta_h(timer_s* t);

#endif


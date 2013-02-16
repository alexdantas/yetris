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

/** @file timer.c
 *  Implements stuff related to the timer.
 *
 *  Basically, it starts a timer, stops it and returns the delta between
 *  them in several time units.
 *
 *  @note If both timer_start() and timer_stop() don't get called
 *        before any timer_delta_*() function, the results are unpredictable.
 */

#include <stdio.h>
#include <sys/time.h>
#include "timer.h"

 /** Save the current time as a start point.
 *
 *  @note Remember to call timer_stop() to record the diff.
 */
int timer_start (struct timert* t)
{
	return gettimeofday (&(t->start), NULL);
}


/** Save the current time as a stop point.
 *
 *  @note Remember to call timer_delta() to retrieve the timer diff.
 */
int timer_stop (struct timert* t)
{
	return gettimeofday (&(t->end), NULL);
}

 /** Returns the time diff in microseconds (1/1000000 seconds) */
long timer_delta_useconds (struct timert* t)
{
	time_t      delta_seconds = (t->end.tv_sec  - t->start.tv_sec);
	suseconds_t delta_micro   = (t->end.tv_usec - t->start.tv_usec);

	return (delta_seconds * 1000000 + delta_micro);
}

 /** Returns the time diff in mili (1/1000 seconds) */
long timer_delta_mseconds(struct timert* t)
{
	return timer_delta_useconds(t) / 1000;
}

 /** Returns the time diff in seconds */
long timer_delta_seconds(struct timert* t)
{
	return timer_delta_useconds(t) / 1000000;
}

 /** Returns the time diff in minutes (60 seconds) */
long timer_delta_minutes(struct timert* t)
{
	return timer_delta_seconds(t) / 60;
}

 /** Returns the time diff in hours (3600 seconds) */
long timer_delta_hours(struct timert* t)
{
	return timer_delta_minutes(t) / 60;
}


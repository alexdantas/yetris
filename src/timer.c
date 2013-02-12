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
 *  Definition of the timer functions.
 */

#include <stdio.h>
#include <sys/time.h>
#include "timer.h"



/** Returns the delta between the start and end of the timer in microseconds.
 *
 *  @note If timer_start() and timer_stop() doesn't get called
 *        before this, the results are unpredictable.
 */


/** Returns the difference between the start and the end points of 't'.
 */
long timer_delta_useconds (struct timert* t)
{
	time_t      delta_seconds = (t->end.tv_sec  - t->start.tv_sec);
	suseconds_t delta_micro   = (t->end.tv_usec - t->start.tv_usec);

	return (delta_seconds * 1000000 + delta_micro);
}

long timer_delta_mseconds(struct timert* t)
{
	return timer_delta_useconds(t) / 1000;
}

long timer_delta_seconds(struct timert* t)
{
	return timer_delta_useconds(t) / 1000000;
}

long timer_delta_minutes(struct timert* t)
{
	return timer_delta_seconds(t) / 60;
}

long timer_delta_hours(struct timert* t)
{
	return timer_delta_minutes(t) / 60;
}

/** Records the current time as a start polong.
 *
 *  Remember to call timer_stop() to record the diff.
 */
int timer_start (struct timert* t)
{
	return gettimeofday (&(t->start), NULL);
}


/** Records the current time as a stop point.
 *
 *  Remember to call timer_delta() to retrieve the timer diff.
 */
int timer_stop (struct timert* t)
{
	return gettimeofday (&(t->end), NULL);
}



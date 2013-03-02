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
 *  Defines stuff related to the timer.
 *
 *  Basically, it starts a timer, stops it and returns the delta between
 *  them in several time units.
 *
 *  @note If both timer_start() and timer_stop() don't get called
 *        before any timer_delta_*() function, the results are unpredictable.
 *
 *  This module uses gettimeofday().
 *  It is Unix, GNU/Linux and Mac OS X system-specific.
 *  The only portable function is time.h's clock(), but it isn't very precise.
 *  See: http://www.songho.ca/misc/timer/timer.html
 */

#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED

#include <sys/time.h>

/** The timer structure */
typedef struct timer_s
{
	struct timeval start; /**< The timer start point */
	struct timeval end;   /**< The timer stop point */
} timer_s;

int  timer_start (timer_s* t);
int  timer_stop (timer_s* t);
long timer_delta_useconds (timer_s* t);
long timer_delta_mseconds(timer_s* t);
long timer_delta_seconds(timer_s* t);
long timer_delta_minutes(timer_s* t);
long timer_delta_hours(timer_s* t);

#endif


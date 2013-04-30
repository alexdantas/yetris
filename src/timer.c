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

/** @file timer.c
 *	Implements stuff related to the timer.
 *
 *	Basically, it starts a timer, stops it and returns the delta between
 *	them in several time units.
 *
 *	@note If both timer_start() and timer_stop() don't get called
 *		  before any timer_delta_*() function, the results are unpredictable.
 */

#include "timer.h"

/* How many microseconds exists in a second */
#define MICRO_IN_SECONDS 1000000

static suseconds_t get_ticks()
{
	struct timeval tmp;
	gettimeofday (&(tmp), NULL);

	return tmp.tv_usec + (tmp.tv_sec * MICRO_IN_SECONDS);
}
void timer_start(timer_s* t)
{
	t->start_mark = get_ticks();
	t->stop_mark  = 0;
	t->pause_mark = 0;
	t->running	  = true;
	t->paused	  = false;
}
void timer_pause(timer_s* t)
{
	if (!(t->running) || (t->paused)) return;

	t->pause_mark = get_ticks() - (t->start_mark);
	t->running	  = false;
	t->paused	  = true;
}
void timer_unpause(timer_s* t)
{
	if (t->running || !(t->paused)) return;

	t->start_mark = get_ticks() - (t->pause_mark);
	t->running	  = true;
	t->paused	  = false;
}
suseconds_t timer_delta_us (timer_s* t)
{
	if (t->running)
		return get_ticks() - (t->start_mark);

	if (t->paused)
		return t->pause_mark;

	// Will never actually get here
	return (t->pause_mark) - (t->start_mark);
}
long timer_delta_ms(timer_s* t)
{
	return timer_delta_us(t) / 1000;
}
long timer_delta_s(timer_s* t)
{
	return timer_delta_us(t) / 1000000;
}
long timer_delta_m(timer_s* t)
{
	return timer_delta_s(t) / 60;
}
long timer_delta_h(timer_s* t)
{
	return timer_delta_m(t) / 60;
}


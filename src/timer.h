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


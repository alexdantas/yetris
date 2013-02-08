/** @file timer.c
 *
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
long timer_delta (struct timert* t)
{
	time_t      delta_seconds = (t->end.tv_sec  - t->start.tv_sec);
	suseconds_t delta_micro   = (t->end.tv_usec - t->start.tv_usec);

	return (delta_seconds * 1000000 + delta_micro);
}


/** Records the current time as a start point.
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




#ifndef GLOBALS_H_DEFINED
#define GLOBALS_H_DEFINED

#include <stdbool.h>

/* I know it's ugly to leave so many global variables >_<
 * please don't sue me */

typedef struct globals_s
{
	int  current_width;
	int  current_height;
	bool screen_center_vertically;
	bool screen_center_horizontally;

} globals_s;

/* global struct containing all global variables.. wow  */
globals_s global;

globals_s new_globals();

#endif /* GLOBALS_H_DEFINED */


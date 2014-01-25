#include "utils.h"
#include <stdlib.h>

/** Returns a random integer between #min and #max */
int random_int_between(int min, int max)
{
	if (min > max) { int tmp = min; min = max; max = tmp; }

	return rand() % (max - min + 1) + min;
}


#include "utils.h"
#include <stdlib.h>

int random_int_between(int min, int max)
{
	if (min > max) { int tmp = min; min = max; max = tmp; }

	return rand() % (max - min + 1) + min;
}

bool random_bool()
{
	/* Is a random number on (1..10) even? */

	return ((random_int_between(1, 10) % 2) == 0);
}


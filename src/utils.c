#include "utils.h"
#include <stdlib.h>

int random_int_between (int upper, int lower)
{
	if (upper < lower)
	{
		int tmp = upper;
		upper = lower;
		lower = tmp;
	}

	return rand() % (upper - lower + 1) + lower;
}


#include "random.h"
#include <stdlib.h>

int Random::getRand(int a, int b)
{
	return rand() % (b - a + 1) + a;
}
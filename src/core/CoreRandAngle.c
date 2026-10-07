// bdc 0x089bede4 CoreRandAngle
#include "bdc.h"

/* Returns a random angle in `[-pi, pi)`: `CoreRandFloat``(2*pi) - pi`. */
float CoreRandAngle(void)
{
    return CoreRandFloat(6.2831855f) - 3.1415927f;
}

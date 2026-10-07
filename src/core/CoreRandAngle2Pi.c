// bdc 0x089bee18 CoreRandAngle2Pi
#include "bdc.h"

/* Returns a random angle in `[0, 2*pi)` (`CoreRandFloat``(2*pi)`, tail call). */
float CoreRandAngle2Pi(void)
{
    return CoreRandFloat(6.2831855f);
}

// bdc 0x089bed84 CoreRandFloat
#include "bdc.h"

/* Returns a uniform random float in `[0, max)`: 18 bits of `CoreRandNext``(0)` scaled by
   about `1/262144` (the constant is `0x3680001c`, slightly above 2^-18) and multiplied by `max`. */
float CoreRandFloat(float max)
{
    u32 bits = CoreRandNext(0) & 0x3ffff;

    return (float)(s32)bits * 0x1.000038p-18f * max;
}

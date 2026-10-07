// bdc 0x089bef88 MathPopCount32
#include "bdc.h"

/* Returns the number of set bits in `x` (SWAR population count). */
int MathPopCount32(u32 x)
{
    x = (x & 0x55555555U) + ((x >> 1) & 0x55555555U);
    x = (x & 0x33333333U) + ((x >> 2) & 0x33333333U);
    x = (x & 0x0f0f0f0fU) + ((x >> 4) & 0x0f0f0f0fU);
    x = (x & 0x00ff00ffU) + ((x >> 8) & 0x00ff00ffU);
    return (int)((x & 0xffffU) + (x >> 16));
}

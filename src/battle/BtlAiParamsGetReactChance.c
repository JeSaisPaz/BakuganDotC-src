// bdc 0x08a2a544 BtlAiParamsGetReactChance
#include "bdc.h"

/* Default per-species AI parameter getter (entry 5, fn at `+0x2c`, base vtable `0x08af6280`): maps
   `level` 1..10 (a compiler jump table) to 0, 0, 0, 0, 10, 10, 10, 20, 20, 20; other
   values give 0. */

s32 BtlAiParamsGetReactChance(BtlAiParams *self, s32 level)
{
    (void)self;
    switch (level) {
    case 5:
    case 6:
    case 7:
        return 10;
    case 8:
    case 9:
    case 10:
        return 20;
    default:
        return 0;
    }
}

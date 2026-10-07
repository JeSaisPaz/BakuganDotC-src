// bdc 0x08a2a4dc BtlAiParamsGetCounterChance
#include "bdc.h"

/* Default per-species AI parameter getter (vtable entry 4, fn at `+0x24`, base vtable
   `0x08af6280`): maps `level` 1..10 through a jump table to 0, 0, 0, 20, 20, 20, 20, 30, 30, 50;
   any other level gives 0. */
s32 BtlAiParamsGetCounterChance(BtlAiParams *self, s32 level)
{
    (void)self;
    switch ((u32)(level - 1)) {
    case 3:
    case 4:
    case 5:
    case 6:
        return 20;
    case 7:
    case 8:
        return 30;
    case 9:
        return 50;
    default:
        return 0;
    }
}

// bdc 0x08a2b2f4 BtlAiParams26PiercianGetCounterChance
#include "bdc.h"

/* Piercian override of the per-species AI parameter slot `+0x24` (vtable `0x08af6a38`): the counter
   chance for `levelIndex` (level − 1), 0 for every level, so a Piercian CPU never counters. The
   compiled code keeps a 10-entry jump table whose every case returns 0. */
s32 BtlAiParams26PiercianGetCounterChance(BtlAiParams *self, s32 levelIndex)
{
    (void)self;
    (void)levelIndex;
    return 0;
}

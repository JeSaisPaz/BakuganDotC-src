// bdc 0x08a2b324 BtlAiParams26PiercianGetReactChance
#include "bdc.h"

/* Piercian override of the per-species AI parameter slot `+0x2c` (vtable `0x08af6a38`): the
   reaction chance for `levelIndex` (skill level − 1, as `BtlAiApplyLevel` passes it), 0 for
   every level, so a Piercian CPU never gets a reaction chance. The compiled code switches on
   `levelIndex - 1` through a 10-entry jump table whose every case, like the out-of-range path,
   returns 0. */
s32 BtlAiParams26PiercianGetReactChance(BtlAiParams *self, s32 levelIndex)
{
    (void)self;
    (void)levelIndex;
    return 0;
}

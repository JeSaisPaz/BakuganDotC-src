// bdc 0x08a2b324 BtlAiParams26PiercianGetReactChance
#include "bdc.h"

/* Piercian override of the per-species AI parameter slot `+0x2c` (vtable `0x08af6a38`): the
   reaction chance for AI `level` (1..10), 0 for every level. The compiled code switches on
   `level - 1` through a 10-entry jump table whose every case, like the out-of-range path,
   returns 0. */
s32 BtlAiParams26PiercianGetReactChance(BtlAiParams *self, s32 level)
{
    (void)self;
    (void)level;
    return 0;
}

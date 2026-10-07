// bdc 0x08a2addc BtlAiParams31FortressGetBlockedMoveChance
#include "bdc.h"

/* Fortress override of the per-species AI parameter slot `+0x1c`: returns 0, so
   `BtlAiRunMoveRules` never switches to movement state 6 when the path is blocked (base value
   50). */
s32 BtlAiParams31FortressGetBlockedMoveChance(BtlAiParams *self)
{
    (void)self;
    return 0;
}

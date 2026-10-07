// bdc 0x08a2aa98 BtlAiParams25BalitonGetBlockedMoveChance
#include "bdc.h"

/* Baliton override of the per-species AI parameter slot `+0x1c`: returns 90, the percent chance
   that `BtlAiRunMoveRules` switches the movement state to 6 when the path is blocked (base
   value 50). */
s32 BtlAiParams25BalitonGetBlockedMoveChance(BtlAiParams *self)
{
    (void)self;
    return 90;
}

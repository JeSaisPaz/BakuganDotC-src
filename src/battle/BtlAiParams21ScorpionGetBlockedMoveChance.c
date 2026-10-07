// bdc 0x08a2b40c BtlAiParams21ScorpionGetBlockedMoveChance
#include "bdc.h"

/* Scorpion override of the per-species AI parameter slot `+0x1c` (vtable `0x08af6ac8`): returns 0,
   so `BtlAiRunMoveRules` never switches to movement state 6 when the path is blocked (base value
   50). */

s32 BtlAiParams21ScorpionGetBlockedMoveChance(BtlAiParams *self)
{
  (void)self;
  return 0;
}

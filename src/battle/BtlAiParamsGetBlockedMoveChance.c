// bdc 0x08a2a4d4 BtlAiParamsGetBlockedMoveChance
#include "bdc.h"

/* Default per-species AI parameter getter (entry 3, fn at `+0x1c`, base vtable `0x08af6280`):
   returns 50, the percent chance used when the AI's movement is blocked. */

s32 BtlAiParamsGetBlockedMoveChance(BtlAiParams *self)
{
  (void)self;
  return 50;
}

// bdc 0x08a2b968 BtlAiParamSetSlot1NpcLevel1DelayWeight
#include "bdc.h"

/* Level-1 entry of the `BtlAiParamSetSlot1Npc` delay weight table (vtable `0x08af6cf0` slot
   `+0x14`): returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 1 —
   100 for index 5 (a 5/30 s delay), 0 for every other index. */

s32 BtlAiParamSetSlot1NpcLevel1DelayWeight(BtlAiParamSet *self, s32 delayIndex)

{
  (void)self;
  return delayIndex == 5 ? 100 : 0;
}

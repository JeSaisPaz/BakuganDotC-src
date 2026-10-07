// bdc 0x08833ae0 BtlHudStepScoreDisplay1Alt
#include "bdc.h"

/* Counter 12 of `BtlHudGetScoreCounterValue`: steps and returns player 1's displayed score via
   `BtlHudStepScoreDisplay``(self, 1)` (byte-identical to `BtlHudStepScoreDisplay1`). */

s32 BtlHudStepScoreDisplay1Alt(BtlHud *self)
{
    return BtlHudStepScoreDisplay(self, 1);
}

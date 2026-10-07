// bdc 0x08833a64 BtlHudStepScoreDisplay2
#include "bdc.h"

/* Counter 2 of `BtlHudGetScoreCounterValue`: returns `BtlHudStepScoreDisplay``(self, 2)`, the
   displayed score of player 2, stepped by 1 toward
   the real score while its count-up flag is set. */
s32 BtlHudStepScoreDisplay2(BtlHud *self)
{
    return BtlHudStepScoreDisplay(self, 2);
}

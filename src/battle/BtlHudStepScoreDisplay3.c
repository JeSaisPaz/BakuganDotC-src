// bdc 0x08833a80 BtlHudStepScoreDisplay3
#include "bdc.h"

/* Counter 3 of `BtlHudGetScoreCounterValue`: returns `BtlHudStepScoreDisplay``(self, 3)`, the
   displayed score of player 3, stepped by 1 toward
   the real score while its count-up flag is set. */
s32 BtlHudStepScoreDisplay3(BtlHud *self)
{
    return BtlHudStepScoreDisplay(self, 3);
}

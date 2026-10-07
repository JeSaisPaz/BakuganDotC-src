// bdc 0x08833a48 BtlHudStepScoreDisplay1
#include "bdc.h"

/* Counter 1 of `BtlHudGetScoreCounterValue`: returns `BtlHudStepScoreDisplay``(self, 1)`, the
   displayed score of player 1, stepped by 1 toward
   the real score while its count-up flag is set. */
s32 BtlHudStepScoreDisplay1(BtlHud *self)
{
    return BtlHudStepScoreDisplay(self, 1);
}

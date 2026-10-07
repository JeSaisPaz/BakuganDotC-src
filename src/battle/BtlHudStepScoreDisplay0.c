// bdc 0x08833a2c BtlHudStepScoreDisplay0
#include "bdc.h"

/* Counter 0 of `BtlHudGetScoreCounterValue`: returns `BtlHudStepScoreDisplay``(self, 0)`, the
   displayed score of player 0, stepped by 1 toward
   the real score while its count-up flag is set. */
s32 BtlHudStepScoreDisplay0(BtlHud *self)
{
    return BtlHudStepScoreDisplay(self, 0);
}

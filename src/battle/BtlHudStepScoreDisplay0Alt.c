// bdc 0x08833ac4 BtlHudStepScoreDisplay0Alt
#include "bdc.h"

/* Counter 10 of `BtlHudGetScoreCounterValue`: `BtlHudStepScoreDisplay``(hud, 0)` again, for the
   second digit layout (byte-identical to `BtlHudStepScoreDisplay0`). */
s32 BtlHudStepScoreDisplay0Alt(BtlHud *self)
{
    return BtlHudStepScoreDisplay(self, 0);
}

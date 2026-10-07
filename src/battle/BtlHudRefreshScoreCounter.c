// bdc 0x08833d80 BtlHudRefreshScoreCounter
#include "bdc.h"

/* Redraws score-board counter `counter` with BtlHudSetScoreDigits on the HUD's sprite list;
   `value` -999 means "use the counter's current value" (BtlHudGetScoreCounterValue). */
void BtlHudRefreshScoreCounter(BtlHud *self, s32 counter, s32 value)
{
    if (value == -999) {
        value = BtlHudGetScoreCounterValue(self, counter);
    }
    BtlHudSetScoreDigits(self, self->sprites, value, counter);
}

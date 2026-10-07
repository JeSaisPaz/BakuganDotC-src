// bdc 0x08863274 BtlBakuganAddClampedHitCount
#include "bdc.h"

/* Adds `delta` to the unit's hit tally and clamps it to at most 10 when `multiplier == 2`, else to
   at most 5 (no lower clamp). */
void BtlBakuganAddClampedHitCount(BtlBakugan *self, int delta, int multiplier)
{
    int tally = self->hitTally + delta;

    self->hitTally = tally;
    if (multiplier == 2) {
        self->hitTally = (tally < 11) ? tally : 10;
        return;
    }
    self->hitTally = (tally < 6) ? tally : 5;
}

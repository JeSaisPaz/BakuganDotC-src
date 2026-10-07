// bdc 0x088600c8 BtlBakuganCommitPendingComboHits
#include "bdc.h"

/* Adds the pending hit count to the unit's current combo counter (capped at 99; the sum is
   compared and truncated as a float), raises the best-combo record when exceeded, and clears the
   pending count. */
void BtlBakuganCommitPendingComboHits(BtlBakugan *self)
{
    float sum = (float)(self->combo + self->pendingComboHits);
    int combo;

    if (sum <= 99.0f) {
        combo = (int)sum;
    } else {
        combo = (int)99.0f;
    }
    self->combo = combo;
    if (self->bestCombo < combo) {
        self->bestCombo = combo;
    }
    self->pendingComboHits = 0;
}

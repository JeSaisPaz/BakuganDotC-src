// bdc 0x08860188 BtlBakuganResetCombo
#include "bdc.h"

/* Ends the unit's combo: clears the combo counter `+0x198` and the combo timer `+0x1a4`; in the
   game mode where `BtlIsScoreMode``(1)` holds it also revalidates the linked unit pointer
   `+0x3bc` with `BtlBakuganListFind`. */
void BtlBakuganResetCombo(BtlBakugan *self)
{
    if (BtlIsScoreMode(1) != 0) {
        self->linkedUnit = BtlBakuganListFind(self->linkedUnit);
    }
    self->combo = 0;
    self->comboTimer = 0;
}

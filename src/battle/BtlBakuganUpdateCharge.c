// bdc 0x08864c18 BtlBakuganUpdateCharge
#include "bdc.h"

/* While command flag 0x400 (the charge button) is held and the unit is not dead, adds
   `100 / stats->chargeFrames` to the charge gauge this frame (`BtlBakuganAddCharge`);
   otherwise resets `charge` to 0. Called from `BtlBakuganUpdate`. */
void BtlBakuganUpdateCharge(BtlBakugan *self)
{
    if ((self->commands & 0x400) != 0 && self->combat.dead == 0) {
        BtlBakuganAddCharge(100.0f / self->combat.stats->chargeFrames, self);
        return;
    }
    self->charge = 0.0f;
}

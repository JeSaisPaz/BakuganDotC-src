// bdc 0x0886000c BtlBakuganAddCharge
#include "bdc.h"

/* Adds `amount` times a multiplier to the unit's charge gauge (`charge`), capped at 100 (a NaN sum
   also stores 100). The multiplier starts at 1.0, becomes 1.5 when the unit's `BtlCombatState`
   has upgrade 0x17, and gets 1.0 added when it has upgrade 0x18 (`BtlCombatHasUpgrade`). */
void BtlBakuganAddCharge(float amount, BtlBakugan *self)
{
    float scale = 1.0f;
    float charge;

    if (BtlCombatHasUpgrade(&self->combat, 0x17) != 0) {
        scale = 1.0f + 0.5f;
    }
    if (BtlCombatHasUpgrade(&self->combat, 0x18) != 0) {
        scale = scale + 1.0f;
    }
    charge = self->charge + scale * amount;
    if (!(charge <= 100.0f)) {
        charge = 100.0f;
    }
    self->charge = charge;
}

// bdc 0x08889dc0 UiHpGaugeGetHp
#include "bdc.h"

/* Returns the current HP the HUD gauge should show: for mode 1 (`gauge->+0x8c`)
   `BtlCombatGetHp``(unit + 0x434)` with the unit at `+0x28`; for mode 2 the int at `+0x2c ->
   +0x200` as float; otherwise 0.0. Counterpart of `UiHpGaugeGetMaxHp`. */

float UiHpGaugeGetHp(UiHpGauge *self)
{
    if (self->mode < 2) {
        if (self->mode > 0) {
            return BtlCombatGetHp(&self->unit->combat);
        }
        return 0.0f;
    }
    if (self->mode < 3) {
        return (float)self->object->hp;
    }
    return 0.0f;
}


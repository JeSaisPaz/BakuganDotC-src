// bdc 0x08889d38 UiHpGaugeGetMaxHp
#include "bdc.h"

/* Returns the maximum HP the HUD gauge should show: for mode 1 (`gauge->+0x8c`)
   `(float)``BtlCombatGetMaxHp``(unit + 0x434)` with the unit at `+0x28`; for mode 2 the int at
   `+0x2c -> +0x204` as float; otherwise 0.0. */

float UiHpGaugeGetMaxHp(UiHpGauge *self)
{
    if (self->mode < 2) {
        if (self->mode > 0) {
            return (float)(u32)BtlCombatGetMaxHp(&self->unit->combat);
        }
        return 0.0f;
    }
    if (self->mode < 3) {
        return (float)self->object->maxHp;
    }
    return 0.0f;
}


// bdc 0x08865c88 BtlBakuganGetScaledStat4C
#include "bdc.h"

/* Returns the unit's base move speed (`moveSpeed`) from its `BtlUnitStatTable` times
   `BtlBakuganGetSpeedStatusFactor`, halved when `slowWalk` is set and the unit's attribute
   (vtable slot 20, as in `BtlBakuganGetAttributeColor`) is not 2. Used by
   `BtlBakuganSetState`, `BtlBakuganState08Update`, `BtlBakuganState11Update`,
   `BtlBakuganDashStep`, `BtlUnitMode4MoveTowardPoint`. */
float BtlBakuganGetScaledStat4C(BtlBakugan *self)
{
    float value = self->combat.stats->moveSpeed;

    value = value * BtlBakuganGetSpeedStatusFactor(self);
    if (self->slowWalk != 0) {
        const VtblEntry *entry = &((const VtblEntry *)self->base.base.vtable)[20];

        if (((int (*)(void *))entry->fn)((u8 *)self + entry->delta) != 2) {
            value = value * 0.5f;
        }
    }
    return value;
}

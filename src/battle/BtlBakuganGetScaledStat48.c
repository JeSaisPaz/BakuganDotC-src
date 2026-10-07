// bdc 0x08865c00 BtlBakuganGetScaledStat48
#include "bdc.h"

/* Returns the unit's dash start-speed stat (`BtlUnitStatTable``.dashStartSpeed`) times
   `BtlBakuganGetSpeedStatusFactor`, halved when `slowWalk` is set and the unit's virtual at
   vtable entry 20 (offset 0xa0) does not return 2. Used by `BtlBakuganSetState`,
   `BtlBakuganState02Update`, `BtlBakuganState07Update` and `BtlBakuganState11Update`. */
float BtlBakuganGetScaledStat48(BtlBakugan *self)
{
    float speed = self->combat.stats->dashStartSpeed;
    u8 halve;

    speed *= BtlBakuganGetSpeedStatusFactor(self);
    halve = 0;
    if (self->slowWalk != 0) {
        const VtblEntry *entry = &((const VtblEntry *)self->base.base.vtable)[20];

        if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 2) {
            halve = 1;
        }
    }
    if (halve) {
        speed *= 0.5f;
    }
    return speed;
}

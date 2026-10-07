// bdc 0x08865af4 BtlBakuganGetMeleeStepSpeed
#include "bdc.h"

/* Returns the forward speed used during the melee/combo states (`BtlBakuganState07Update`,
   `BtlBakuganState11Update`): the stat table's `meleeStepSpeed` times
   `BtlBakuganGetSpeedStatusFactor`, halved when `slowWalk` is set and the unit's attribute
   (virtual slot 20, `+0xa0`) is not 2. */
float BtlBakuganGetMeleeStepSpeed(BtlBakugan *self)
{
    float speed = self->combat.stats->meleeStepSpeed;
    bool halve = false;

    speed = speed * BtlBakuganGetSpeedStatusFactor(self);
    if (self->slowWalk != 0) {
        const VtblEntry *entry = &((const VtblEntry *)self->base.base.vtable)[20];
        int attribute = ((int (*)(void *))entry->fn)((u8 *)self + entry->delta);

        if (attribute != 2) {
            halve = true;
        }
    }
    if (halve) {
        speed = speed * 0.5f;
    }
    return speed;
}

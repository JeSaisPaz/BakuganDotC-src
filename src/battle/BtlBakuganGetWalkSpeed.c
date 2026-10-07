// bdc 0x08865a24 BtlBakuganGetWalkSpeed
#include "bdc.h"

/* Returns the walking speed of the movement state (`BtlBakuganState01Update`): the stat table's
   `walkSpeed` times `BtlBakuganGetSpeedStatusFactor`. When `slowWalk` is set, the unit's
   attribute (virtual slot 20, `+0xa0`) is not 2 and the kind is not 0xf, the speed is halved, and
   halved again when a second call of that virtual returns 5. */
float BtlBakuganGetWalkSpeed(BtlBakugan *self)
{
    float speed = self->combat.stats->walkSpeed;
    bool slow = false;
    const VtblEntry *entry;

    speed = speed * BtlBakuganGetSpeedStatusFactor(self);
    if (self->slowWalk != 0) {
        entry = &((const VtblEntry *)self->base.base.vtable)[20];
        if (((int (*)(void *))entry->fn)((u8 *)self + entry->delta) != 2) {
            slow = true;
        }
    }
    if (slow && self->base.base.unk08 != 0xf) {
        speed = speed * 0.5f;
        entry = &((const VtblEntry *)self->base.base.vtable)[20];
        if (((int (*)(void *))entry->fn)((u8 *)self + entry->delta) == 5) {
            speed = speed * 0.5f;
        }
    }
    return speed;
}

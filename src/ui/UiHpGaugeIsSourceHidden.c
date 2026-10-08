// bdc 0x0888b318 UiHpGaugeIsSourceHidden
#include "bdc.h"

/* Returns whether the HUD hit-point gauge (`UiHpGaugeInit`, 0xa0 bytes) must be hidden: mode 1
   → for a non-local unit, whether its status 9 is active (`unit+0x476` = status slot 9 `active`);
   mode 2 → whether the object's virtual `+0x94` holds, in which case it also zeroes the gauge
   alpha `+0x80`. Used by `UiHpGaugeUpdate`. */

u8 UiHpGaugeIsSourceHidden(UiHpGauge *self)
{
    u8 result = 0;

    if (self->mode < 2) {
        if (self->mode > 0 && !BtlBakuganIsLocalPlayer(self->unit)) {
            result = self->unit->combat.status[9].active;
        }
    } else if (self->mode < 3) {
        const VtblEntry *e = &((const VtblEntry *)self->object->base.base.vtable)[18];
        if (((s32 (*)(void *))e->fn)((u8 *)self->object + e->delta) != 0) {
            result = 1;
            self->fade = 0.0f;
        }
    }
    return result;
}

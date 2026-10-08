// bdc 0x0888b8b8 UiHpGaugeObjectSlot6C
#include "bdc.h"

/* Returns 1 when the HUD hit-point gauge (`UiHpGaugeInit`, 0xa0 bytes) is bound to an object
   (mode 2) whose virtual `+0x6c` holds, else 0. `UiHpGaugeDraw` positions such gauges from the
   object's field `+0x21c`, others are skipped in that pass. */

s32 UiHpGaugeObjectSlot6C(UiHpGauge *self)
{
    s32 result = 0;
    if (self->mode >= 2 && self->mode < 3) {
        const VtblEntry *e = ((const VtblEntry *)self->object->base.base.vtable) + 0xd;
        if (((s32 (*)(void *))e->fn)((char *)self->object + e->delta) != 0) {
            result = 1;
        }
    }
    return result;
}


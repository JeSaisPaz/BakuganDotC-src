// bdc 0x0888a004 UiHpGaugeGetBarStyle
#include "bdc.h"

/* Returns the bar style of the HUD hit-point gauge (`UiHpGaugeInit`, 0xa0 bytes) used by
   `UiHpGaugeEmitBars`: mode 1 (unit source `+0x28`) → 1 when the unit's virtual `+0x5c` holds,
   else 0; mode 2 (object source `+0x2c`) → 1 when neither of the object's virtuals `+0x6c` and
   `+0x64` holds, else 2; 0 for other modes. */

s32 UiHpGaugeGetBarStyle(UiHpGauge *self)
{
    if (self->mode < 2) {
        if (self->mode > 0) {
            const VtblEntry *e = &((const VtblEntry *)self->unit->base.base.vtable)[11];
            return ((s32 (*)(void *))e->fn)((u8 *)self->unit + e->delta) != 0;
        }
        return 0;
    }
    if (self->mode < 3) {
        const VtblEntry *e = &((const VtblEntry *)self->object->base.base.vtable)[13];
        s32 style = 1;
        if (((s32 (*)(void *))e->fn)((u8 *)self->object + e->delta) != 0) {
            return 2;
        }
        e = &((const VtblEntry *)self->object->base.base.vtable)[12];
        if (((s32 (*)(void *))e->fn)((u8 *)self->object + e->delta) != 0) {
            style = 2;
        }
        return style;
    }
    return 0;
}

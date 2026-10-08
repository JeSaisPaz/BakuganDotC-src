// bdc 0x0888a118 UiHpGaugeGetBarWidth
#include "bdc.h"

/* Returns the on-screen bar length of the HUD HP gauge (0xa0-byte node,
   `UiHpGaugeInit`/`UiHpGaugeCtorForObject`, mode `+0x8c`): proportional to the max HP (`+0x70 *
   80 / 1000`, clamped to 20..80), or `* 0.7 + 24` clamped to 24..80 for a mode-2 stage object
   whose virtual `+0x6c` or `+0x64` returns nonzero; for the local player's unit
   (`UiHpGaugeIsLocalPlayer`) it is `((``UiHpGaugeGetMaxHp`` / stats->levelHp[4] - 0.5) * 0.7
   + 0.3) * 177 + 24`, unclamped. */

float UiHpGaugeGetBarWidth(UiHpGauge *self)
{
    float width;

    if (self->mode == 2) {
        const VtblEntry *e = &((const VtblEntry *)self->object->base.base.vtable)[13];
        if (((s32 (*)(void *))e->fn)((u8 *)self->object + e->delta) == 0) {
            e = &((const VtblEntry *)self->object->base.base.vtable)[12];
            if (((s32 (*)(void *))e->fn)((u8 *)self->object + e->delta) == 0) {
                width = self->maxHp * 80.0f * 0.001f;
                if (width < 20.0f) {
                    return 20.0f;
                }
                if (!(width <= 80.0f)) {
                    width = 80.0f;
                }
                return width;
            }
        }
        width = self->maxHp * 80.0f * 0.001f * 0.7f + 24.0f;
        if (width < 24.0f) {
            return 24.0f;
        }
        if (!(width <= 80.0f)) {
            width = 80.0f;
        }
        return width;
    }
    if (UiHpGaugeIsLocalPlayer(self)) {
        float maxHp = UiHpGaugeGetMaxHp(self);
        return ((maxHp / self->unit->combat.stats->levelHp[4] - 0.5f) * 0.7f + 0.3f) * 177.0f + 24.0f;
    }
    width = self->maxHp * 80.0f * 0.001f;
    if (width < 20.0f) {
        return 20.0f;
    }
    if (width <= 80.0f) {
        return width;
    }
    return 80.0f;
}

// bdc 0x0888b3c0 UiHpGaugeUpdate
#include "bdc.h"

/* Per-frame update of the HUD HP gauge (0xa0-byte node,
   `UiHpGaugeInit`/`UiHpGaugeCtorForObject`, mode `+0x8c`): copies the bound source's position
   (`GfxModel.pos`, `+0x20`) to the anchor `+0x60`; mode 1 raises its y by 100 when the unit's
   virtual `+0x5c` returns nonzero, else by the stat-table height (`stats->height`); mode 2 replaces
   y with the object's `gaugeAnchor[1]` (`+0x2a4`). Sets `updated` (`+0x90`). While
   `g_btlBattleOver` is set the alpha `+0x84` is just source alpha (`+0x6c`) × fade; otherwise the
   fade `+0x80` moves by 0.02 per frame (down to 0 while `UiHpGaugeIsSourceDown` or
   `UiHpGaugeIsSourceHidden` hold, else up to 1) and the alpha is source alpha × fade, or 1.0 when
   `forceOpaque` (`+0x91`) is set. Then tracks the HP from `UiHpGaugeGetHp`: a rise refreshes
   `maxHp` from `UiHpGaugeGetMaxHp` (raised to the hp if lower), a drop restarts `hitTimer` at
   1.0; the timer falls by 0.013 per frame, the trail `+0x78` snaps to the hp once it is <= 0.2
   and slides towards it by 4% of the hit size per frame once it is <= 0.55. Finally, below 30%
   HP the low-HP flash `+0x94` pulses as `0.3 + 0.2 * sin(phase)` (phase `+0x98` += 0.174,
   wrapped at 2π), else it is 0. */

void UiHpGaugeUpdate(UiHpGauge *self)
{
    GfxModel *src;
    float hp;
    float fade;
    float v;

    self->updated = 0;
    src = (GfxModel *)self->source;
    self->anchor[0] = src->pos[0];
    self->anchor[1] = src->pos[1];
    self->anchor[2] = src->pos[2];
    self->anchor[3] = src->pos[3];
    if (self->mode < 2) {
        if (self->mode > 0) {
            const VtblEntry *e = &((const VtblEntry *)self->unit->base.base.vtable)[11];
            if (((s32 (*)(void *))e->fn)((u8 *)self->unit + e->delta) != 0) {
                self->anchor[1] = self->anchor[1] + 100.0f;
            } else {
                self->anchor[1] = self->anchor[1] + self->unit->combat.stats->height;
            }
        }
    } else if (self->mode < 3) {
        self->anchor[1] = self->object->gaugeAnchor[1];
    }
    self->updated = 1;

    if (g_btlBattleOver != 0) {
        self->alpha = ((GfxModel *)self->source)->ambient[3] * self->fade;
    } else {
        if (UiHpGaugeIsSourceDown(self) || UiHpGaugeIsSourceHidden(self)) {
            fade = self->fade - 0.02f;
            self->fade = fade;
            if (fade < 0.0f) {
                self->fade = 0.0f;
            }
        } else {
            fade = self->fade + 0.02f;
            self->fade = fade;
            if (!(fade <= 1.0f)) {
                self->fade = 1.0f;
            }
        }
        if (self->forceOpaque) {
            self->alpha = 1.0f;
        } else {
            self->alpha = ((GfxModel *)self->source)->ambient[3] * self->fade;
        }
    }

    hp = UiHpGaugeGetHp(self);
    if (self->hp != hp) {
        if (self->hp < hp) {
            self->maxHp = UiHpGaugeGetMaxHp(self);
            v = UiHpGaugeGetHp(self);
            self->hp = v;
            if (self->maxHp < v) {
                self->maxHp = self->hp;
            }
        } else {
            self->hitTimer = 1.0f;
        }
        self->hp = hp;
    }
    if (self->hitTimer <= 0.0f) {
        self->hpTrail = hp;
        self->hpBeforeHit = hp;
    } else {
        v = self->hitTimer - 0.013f;
        self->hitTimer = v;
        if (v <= 0.2f) {
            self->hpTrail = hp;
            self->hpBeforeHit = hp;
        }
        if (self->hitTimer <= 0.55f) {
            v = self->hpTrail - (self->hpBeforeHit - hp) * 0.04f;
            self->hpTrail = v;
            if (v < hp) {
                self->hpTrail = hp;
            }
        }
    }

    hp = UiHpGaugeGetHp(self);
    if (hp / UiHpGaugeGetMaxHp(self) < 0.3f) {
        v = self->lowHpPhase + 0.174f;
        self->lowHpPhase = v;
        if (!(v <= 6.2831855f)) {
            self->lowHpPhase = self->lowHpPhase - 6.2831855f;
        }
        v = __builtin_sinf(self->lowHpPhase);
        self->lowHpFlash = v * 0.2f + 0.3f;
    } else {
        self->lowHpFlash = 0.0f;
    }
}

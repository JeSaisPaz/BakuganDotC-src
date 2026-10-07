// bdc 0x0887975c BtlAttackResolveClash
#include "bdc.h"

/* Common cancellation check of projectile attacks. Returns 0 (keep going) for attacks of category
   0xc (`BtlAttackGetCategory`) and for attacks neither flagged `cancelled` nor clashing
   (`BtlAttackCheckClash`). Otherwise, by the flags (the code's behaviour; the field names come
   from the struct definition): with `reflect` (+0x116) set it plays the hit sound
   (`BtlAttackPlayHitSound`), unless `noSpark` turns `pos` into the unit direction
   normalize(pos - auxVec) in place (each lane clamped to [-1, 1], w set to 0) and spawns the directed spark 0x38 at `auxVec` along it
   (`GfxEffectSpawnDirected`), then ends (`BtlAttackEnd`) and returns 1; else with
   `silentEnd` (+0x115) set it calls `BtlAttackReflect` and returns 0; else it spawns
   `impactEffect` at `pos` on the unit or attack effect manager
   (`BtlAttackTypeUsesStageEffects`), plays the hit sound, ends and returns 1. A
   zero-length vector is scaled by 0. */
int BtlAttackResolveClash(BtlAttack *self, int impactEffect)
{
    float lenSq;
    float scale;

    if (BtlAttackGetCategory(self) == 0xc) {
        return 0;
    }
    if (self->cancelled == 0 && BtlAttackCheckClash(self) == 0) {
        return 0;
    }
    if (self->reflect != 0) {
        BtlAttackPlayHitSound(self);
        if (self->noSpark == 0) {
            /* pos.xyz -= auxVec.xyz, then pos = clamp(normalize(pos.xyz), -1, 1) with w 0
               (the w lane comes from the bank's S713 = 0; a zero-length vector scales by 0). */
            self->pos[0] = self->pos[0] - self->auxVec[0];
            self->pos[1] = self->pos[1] - self->auxVec[1];
            self->pos[2] = self->pos[2] - self->auxVec[2];
            lenSq = self->pos[0] * self->pos[0] + self->pos[1] * self->pos[1] +
                    self->pos[2] * self->pos[2];
            if (lenSq == 0.0f) {
                scale = 0.0f;
            } else {
                scale = VfRsq(lenSq);
            }
            self->pos[0] = VfSat1(self->pos[0] * scale);
            self->pos[1] = VfSat1(self->pos[1] * scale);
            self->pos[2] = VfSat1(self->pos[2] * scale);
            self->pos[3] = 0.0f;
            GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0x38, self->auxVec, self->pos);
        }
        BtlAttackEnd(self);
        return 1;
    }
    if (self->silentEnd != 0) {
        BtlAttackReflect(self);
        return 0;
    }
    if (BtlAttackTypeUsesStageEffects(self, self->type) != 0) {
        GfxEffectSpawn(g_btlUnitEffectMgr, impactEffect, self->pos);
    } else {
        GfxEffectSpawn(g_btlAttackEffectMgr, impactEffect, self->pos);
    }
    BtlAttackPlayHitSound(self);
    BtlAttackEnd(self);
    return 1;
}

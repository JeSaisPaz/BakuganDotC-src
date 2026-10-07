// bdc 0x0887b8e4 BtlAttackUpdateStraightShot
#include "bdc.h"

/* Shared update of a non-homing shot with attached trail effect 0x1d1 (attack types 8 and 0x8d).
   Past 60 frames it ends (`BtlAttackEnd`). On the first frame scales `vel.xyz` by 45 (w set to
   0, bank S713). Later: when `cancelled` or clashing
   (`BtlAttackCheckClash`) spawns effect 0x1a at `pos` and ends; else sweeps for hits along
   `vel` (`BtlAttackSweepHit` with `hitId`, flags 3); on a hit normalises `vel`, spawns the
   directed impact 0x9f at `g_btlAttackHitPoint` (`GfxEffectSpawnDirected`) carrying the
   owner and its id, and ends; otherwise advances `pos` by `vel`. Every ending stops the trail
   effect attached to `pos` (`GfxEffectStopAttached`). */
void BtlAttackUpdateStraightShot(BtlAttack *self, s32 hitId)
{
    GfxEffect *impact;
    float lenSq;
    float k;

    if (self->age > 0x3c) {
        BtlAttackEnd(self);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x1d1, self->pos);
        return;
    }
    if (self->age == 0) {
        /* vel.xyz *= 45; lane w gets bank S713 (0) */
        self->vel[0] = self->vel[0] * 45.0f;
        self->vel[1] = self->vel[1] * 45.0f;
        self->vel[2] = self->vel[2] * 45.0f;
        self->vel[3] = 0.0f;
        return;
    }
    if (self->cancelled != 0 || BtlAttackCheckClash(self) != 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0x1a, self->pos);
        BtlAttackEnd(self);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x1d1, self->pos);
        return;
    }
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitId, 3, 0, 0x31bf337e) != 0) {
        /* vel.xyz normalised and clamped to [-1, 1] (0 when the length is zero); w = bank S713 (0) */
        lenSq = self->vel[0] * self->vel[0] + self->vel[1] * self->vel[1] + self->vel[2] * self->vel[2];
        k = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            k = 0.0f;
        }
        self->vel[0] = VfSat1(self->vel[0] * k);
        self->vel[1] = VfSat1(self->vel[1] * k);
        self->vel[2] = VfSat1(self->vel[2] * k);
        self->vel[3] = 0.0f;
        impact = GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0x9f, &g_btlAttackHitPoint.x, self->vel);
        impact->ownerBakugan = self->owner;
        if (self->owner != NULL) {
            impact->ownerId = self->owner->base.base.id;
        }
        BtlAttackEnd(self);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x1d1, self->pos);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}

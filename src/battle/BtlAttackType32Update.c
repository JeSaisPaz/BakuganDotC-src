// bdc 0x0887ecd8 BtlAttackType32Update
#include "bdc.h"

/* Per-frame handler of attack type 0x32 (handler table `0x08a685f0`, run by `BtlAttackUpdate`).
   After age 90 it stops its trail effect 0x1d1 and ends (`BtlAttackEnd`). On the first frame it
   snaps its direction onto the target (`BtlAttackSteerToTarget` with turn rate 1, speed 0,
   height 80) and then sets the turn rate to 0. Afterwards: when `BtlAttackResolveClash` (impact
   0x1b) consumed it, it stops the trail; else it steers at speed 50 / height 80 and sweeps pos along
   vel for hits (`BtlAttackSweepHit`, hit kind 0x55): on a hit it queues impact 0x1b at the hit
   point (`BtlAttackSetPendingHit`) and stops the trail, otherwise it copies its direction (all
   four lanes) to its effect's `dir` and advances pos.xyz by vel.xyz (w kept). */

void BtlAttackType32Update(BtlAttack *self)
{
    float *pos;
    float *vel;
    GfxEffect *effect;

    if (self->age > 90) {
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x1d1, self->pos);
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->turnRate = 1.0f;
        BtlAttackSteerToTarget(0.0f, 80.0f, self, 1, NULL);
        self->turnRate = 0.0f;
        return;
    }
    pos = self->pos;
    if (BtlAttackResolveClash(self, 0x1b) != 0) {
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x1d1, pos);
        return;
    }
    BtlAttackSteerToTarget(50.0f, 80.0f, self, 1, NULL);
    vel = self->vel;
    if (BtlAttackSweepHit(self->radius, self, pos, vel, 0x55, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0x1b, &g_btlAttackHitPoint.x);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x1d1, pos);
        return;
    }
    effect = (GfxEffect *)self->effect;
    effect->dir[0] = self->dir[0];
    effect->dir[1] = self->dir[1];
    effect->dir[2] = self->dir[2];
    effect->dir[3] = self->dir[3];
    pos[0] = pos[0] + vel[0];
    pos[1] = pos[1] + vel[1];
    pos[2] = pos[2] + vel[2];
}

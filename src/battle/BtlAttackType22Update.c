// bdc 0x0887e108 BtlAttackType22Update
#include "bdc.h"

/* Per-frame handler of attack type 0x22 (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   homing shot. After frame 60 it ends (`BtlAttackEnd`); the first frame sets the turn rate to
   0.03. Later frames, unless `BtlAttackResolveClash` (impact 0xc5) consumed it, home at speed 70
   with a 60-unit height offset (`BtlAttackSteerToTarget`) and sweep along the velocity
   (`BtlAttackSweepHit`, hit id 0x45, kind 3); a hit queues impact 0xc5 at
   `g_btlAttackHitPoint` (`BtlAttackSetPendingHit`). Otherwise it spawns three trail effects
   0xc4 on `g_btlAttackEffectMgr` (`GfxEffectSpawn`) at `pos` + k x 0.334 x `vel` (k = 0..2,
   trail w = pos w) and moves `pos` by `vel` (xyz, w kept). */
void BtlAttackType22Update(BtlAttack *self)
{
    float trail[4];
    float step[3];
    float *pos;
    float *vel;
    int i;

    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->turnRate = 0.03f;
        return;
    }
    if (BtlAttackResolveClash(self, 0xc5) != 0) {
        return;
    }
    BtlAttackSteerToTarget(70.0f, 60.0f, self, 1, NULL);
    pos = self->pos;
    vel = self->vel;
    if (BtlAttackSweepHit(self->radius, self, pos, vel, 0x45, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xc5, &g_btlAttackHitPoint.x);
        return;
    }
    trail[0] = pos[0];
    trail[1] = pos[1];
    trail[2] = pos[2];
    trail[3] = pos[3];
    /* step.w (stale S713 in the original) is never read: left out. */
    step[0] = vel[0] * 0.334f;
    step[1] = vel[1] * 0.334f;
    step[2] = vel[2] * 0.334f;
    for (i = 0; i < 3; i++) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0xc4, trail);
        trail[0] = trail[0] + step[0];
        trail[1] = trail[1] + step[1];
        trail[2] = trail[2] + step[2];
    }
    pos[0] = pos[0] + vel[0];
    pos[1] = pos[1] + vel[1];
    pos[2] = pos[2] + vel[2];
}

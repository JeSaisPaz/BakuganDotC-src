// bdc 0x08880eac BtlAttackType58Update
#include "bdc.h"

/* Per-frame handler of attack type 0x58 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a homing shot. Past age 60 it ends (`BtlAttackEnd`); on frame 0 it does nothing. Later, unless
   `BtlAttackResolveClash` (impact effect 0x1a) consumed it, it homes at speed 40 with an 80-unit
   height offset (`BtlAttackSteerToTarget`), sweeps the hit test along `vel`
   (`BtlAttackSweepHit`, hit kind 0x7b) and on a hit queues impact effect 0x1a at the hit point
   (`BtlAttackSetPendingHit`); otherwise the position advances by `vel` (xyz, w kept). */

void BtlAttackType58Update(BtlAttack *self)
{
    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0 || BtlAttackResolveClash(self, 0x1a) != 0) {
        return;
    }
    BtlAttackSteerToTarget(40.0f, 80.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x7b, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0x1a, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (vadd.t; w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

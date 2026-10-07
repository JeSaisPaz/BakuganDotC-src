// bdc 0x0887fd3c BtlAttackType42Update
#include "bdc.h"

/* Per-frame handler of attack type 0x42 (run by `BtlAttackUpdate`): a homing shot. Once its age
   exceeds 60 frames it ends (`BtlAttackEnd`). Otherwise, after the first frame and unless
   `BtlAttackResolveClash` (impact 0x1b) consumed it, it homes at speed 40 with an 80-unit
   height offset (`BtlAttackSteerToTarget`), sweeps for hits along its velocity
   (`BtlAttackSweepHit`, hit kind 0x65, 3) and either queues impact 0x1b at the hit point
   (`BtlAttackSetPendingHit`) or advances its position by its velocity (xyz, w kept). */

void BtlAttackType42Update(BtlAttack *self)
{
    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0 || BtlAttackResolveClash(self, 0x1b) != 0) {
        return;
    }
    BtlAttackSteerToTarget(40.0f, 80.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x65, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0x1b, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (vadd.t; w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

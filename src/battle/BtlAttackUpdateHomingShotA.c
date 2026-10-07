// bdc 0x0887a04c BtlAttackUpdateHomingShotA
#include "bdc.h"

/* Shared update of a 60-frame homing shot (attack types 0 and 0x89): ends once its age is above
   60. On frame 0 it does nothing. Later, unless `BtlAttackResolveClash` (impact 0x1a) consumed
   it, it homes at speed 40 with an 80-unit height offset (`BtlAttackSteerToTarget`), sweeps for
   hits from `pos` along `vel` (`BtlAttackSweepHit` with hit kind `hitId`) and on a hit queues
   impact 0x1a at the hit point (`BtlAttackSetPendingHit`); otherwise `pos.xyz += vel.xyz`. */

void BtlAttackUpdateHomingShotA(BtlAttack *self, s32 hitId)
{
    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0 || BtlAttackResolveClash(self, 0x1a) != 0) {
        return;
    }
    BtlAttackSteerToTarget(40.0f, 80.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitId, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0x1a, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

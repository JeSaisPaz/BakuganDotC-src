// bdc 0x0888219c BtlAttackType72Update
#include "bdc.h"

/* Per-frame handler of attack type 0x72 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a homing shot. On frame 0 it sets the turn rate to 0.08 and does nothing else; past age 60 it
   ends (`BtlAttackEnd`). Otherwise, unless `BtlAttackResolveClash` (impact effect 0x1b)
   consumed it, it homes at speed 35 with an 80-unit height offset (`BtlAttackSteerToTarget`),
   sweeps the hit test along `vel` (`BtlAttackSweepHit`, hit kind 0x95) and on a hit queues
   impact effect 0x1b at the hit point (`BtlAttackSetPendingHit`); otherwise the position
   advances by `vel` (xyz, w kept). */

void BtlAttackType72Update(BtlAttack *self)
{
    if (self->age == 0) {
        self->turnRate = 0.0799999982f;
    }
    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0 || BtlAttackResolveClash(self, 0x1b) != 0) {
        return;
    }
    BtlAttackSteerToTarget(35.0f, 80.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x95, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0x1b, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

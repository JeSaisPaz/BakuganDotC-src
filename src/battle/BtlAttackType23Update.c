// bdc 0x0887e2ac BtlAttackType23Update
#include "bdc.h"

/* Per-frame handler of attack type 0x23 (run by `BtlAttackUpdate`): a slow-turning homing shot.
   After 90 frames of age it ends (`BtlAttackEnd`). On frame 0 it sets the turn rate to 0.01.
   Later, unless `BtlAttackResolveClash` (impact effect 0xc5) consumed it, it homes at speed 40
   with height offset 60 (`BtlAttackSteerToTarget`), sweeps the hit test along its velocity
   (`BtlAttackSweepHit`, hit kind 0x46) and on a hit queues impact effect 0xc5 at the hit point
   (`BtlAttackSetPendingHit`); without a hit the position advances by the velocity (xyz). */
void BtlAttackType23Update(BtlAttack *self)
{
    if (self->age > 90) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->turnRate = 0.01f;
        return;
    }
    if (BtlAttackResolveClash(self, 0xc5) != 0) {
        return;
    }
    BtlAttackSteerToTarget(40.0f, 60.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x46, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xc5, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

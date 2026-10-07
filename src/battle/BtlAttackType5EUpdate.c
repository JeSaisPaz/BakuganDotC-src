// bdc 0x08881670 BtlAttackType5EUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x5e (run by `BtlAttackUpdate`): a homing shot. After 150
   frames of age it ends (`BtlAttackEnd`). On frame 0 it sets the turn rate to 0.05. Later,
   unless `BtlAttackResolveClash` (impact effect 0x1a) consumed it, it homes at speed 40 with
   height offset 80 (`BtlAttackSteerToTarget`), sweeps the hit test along its velocity
   (`BtlAttackSweepHit`, hit kind 0x81) and on a hit queues impact effect 0x1a at the hit point
   (`BtlAttackSetPendingHit`); without a hit the position advances by the velocity (xyz). */
void BtlAttackType5EUpdate(BtlAttack *self)
{
    if (self->age > 150) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->turnRate = 0.05f;
        return;
    }
    if (BtlAttackResolveClash(self, 0x1a) != 0) {
        return;
    }
    BtlAttackSteerToTarget(40.0f, 80.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x81, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0x1a, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

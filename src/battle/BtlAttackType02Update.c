// bdc 0x0887ae7c BtlAttackType02Update
#include "bdc.h"

/* Per-frame handler of attack type 2 (run by `BtlAttackUpdate`): a homing shot. On frame 0 it
   sets the turn rate to 0.08. After 60 frames of age it ends (`BtlAttackEnd`). On frames 1..60,
   unless `BtlAttackResolveClash` (impact effect 0x1b) consumed it, it homes at speed 35 with
   height offset 80 (`BtlAttackSteerToTarget`), sweeps the hit test along its velocity
   (`BtlAttackSweepHit`, hit kind 0x25) and on a hit queues impact effect 0x1b at the hit point
   (`BtlAttackSetPendingHit`); without a hit the position advances by the velocity (xyz). */
void BtlAttackType02Update(BtlAttack *self)
{
    if (self->age == 0) {
        self->turnRate = 0.0799999982f;
    }
    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        return;
    }
    if (BtlAttackResolveClash(self, 0x1b) != 0) {
        return;
    }
    BtlAttackSteerToTarget(35.0f, 80.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x25, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0x1b, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

// bdc 0x08881bfc BtlAttackType6CUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x6c (run by `BtlAttackUpdate`): a homing shot. After 60
   frames of age it ends (`BtlAttackEnd`); on frame 0 it does nothing. Otherwise, unless
   `BtlAttackResolveClash` (impact effect 0xc5) consumed it, it homes at speed 40 with height
   offset 80 (`BtlAttackSteerToTarget`), sweeps the hit test along its velocity
   (`BtlAttackSweepHit`, hit kind 0x8f) and on a hit queues impact effect 0xc5 at the hit point
   (`BtlAttackSetPendingHit`); without a hit the position advances by the velocity (xyz). */
void BtlAttackType6CUpdate(BtlAttack *self)
{
    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        return;
    }
    if (BtlAttackResolveClash(self, 0xc5) != 0) {
        return;
    }
    BtlAttackSteerToTarget(40.0f, 80.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x8f, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xc5, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

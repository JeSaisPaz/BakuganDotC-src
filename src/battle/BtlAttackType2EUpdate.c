// bdc 0x0887e774 BtlAttackType2EUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x2e (entry 46 of the attack handler table run by
   `BtlAttackUpdate`): a homing shot. After 60 frames of age it ends (`BtlAttackEnd`). On frame 0
   it does nothing; on later frames, unless `BtlAttackResolveClash` (impact effect 0xbb) consumed
   it, it homes at speed 35 with an 80-unit height offset and turn decay
   (`BtlAttackSteerToTarget`), sweeps the hit test along `vel` (hit kind 0x51, arg 3, mask
   `0x31bf337e`, `BtlAttackSweepHit`) and on a hit queues impact 0xbb at `g_btlAttackHitPoint`
   (`BtlAttackSetPendingHit`); otherwise the position advances by `vel` (x, y, z; w kept). */

void BtlAttackType2EUpdate(BtlAttack *self)
{
    float *pos;

    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0 || BtlAttackResolveClash(self, 0xbb) != 0) {
        return;
    }
    BtlAttackSteerToTarget(35.0f, 80.0f, self, 1, NULL);
    pos = self->pos;
    if (BtlAttackSweepHit(self->radius, self, pos, self->vel, 0x51, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xbb, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    pos[0] = pos[0] + self->vel[0];
    pos[1] = pos[1] + self->vel[1];
    pos[2] = pos[2] + self->vel[2];
}

// bdc 0x0887fc04 BtlAttackType41Update
#include "bdc.h"

/* Per-frame handler of attack type 0x41 (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   homing shot with a slow turn rate (0.01); after the first frame, unless `BtlAttackResolveClash`
   (impact 0xb8) consumed it, homes at speed 150 with a 60-unit height offset
   (`BtlAttackSteerToTarget`), sweeps for hits along its velocity (`BtlAttackSweepHit`, hit id
   0x64, kind 3) queuing impact 0xb8 (`BtlAttackSetPendingHit`), and advances. Ends after 60
   frames. */

void BtlAttackType41Update(BtlAttack *self)
{
    if (!((float)self->age <= 60.0f)) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->turnRate = 0.01f;
        return;
    }
    if (BtlAttackResolveClash(self, 0xb8) != 0) {
        return;
    }
    BtlAttackSteerToTarget(150.0f, 60.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 100, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xb8, &g_btlAttackHitPoint.x);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

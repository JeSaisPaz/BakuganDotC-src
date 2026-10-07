// bdc 0x0887ff74 BtlAttackUpdateRisingHomingShot
#include "bdc.h"

/* Shared update of attack types 0x26 and 0x8b. After 90 frames it ends (`BtlAttackEnd`). On
   the first frame variants 1 and 2 (`paramF2` truncated) zero the turn rate and start 60 units
   higher. Afterwards, unless `BtlAttackResolveClash` (impact 0xc5) consumed it, it homes at
   speed 40 / height 60 (`BtlAttackSteerToTarget`) and sweeps from `pos` along `vel` for hits
   (`hitId`, kind 3, `BtlAttackSweepHit`): a hit queues impact 0xc5 at
   `g_btlAttackHitPoint` (`BtlAttackSetPendingHit`), otherwise `pos.xyz += vel.xyz` (w kept). */
void BtlAttackUpdateRisingHomingShot(BtlAttack *self, s32 hitId)
{
    s32 variant;

    if (self->age > 90) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        variant = (s32)self->paramF2;
        if (variant > 0 && variant < 3) {
            self->turnRate = 0.0f;
            self->pos[1] = self->pos[1] + 60.0f;
        }
        return;
    }
    if (BtlAttackResolveClash(self, 0xc5) != 0) {
        return;
    }
    BtlAttackSteerToTarget(40.0f, 60.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitId, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xc5, &g_btlAttackHitPoint.x);
        return;
    }
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}

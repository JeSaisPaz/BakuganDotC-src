// bdc 0x088817cc BtlAttackType68Update
#include "bdc.h"

/* Per-frame handler of attack type 0x68 (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   slow homing orb. Once its age exceeds 150 frames it ends
   (`BtlAttackEnd`); frame 0 does nothing. Otherwise it homes at speed 20 / height 150
   (`BtlAttackSteerToTarget` with turn decay), tests clashes (`BtlAttackCheckClash`) and sweeps
   for hits along `vel` (`BtlAttackSweepHit`, hit kind 0x8b); on a hit it plays sound 0x200099
   (`BtlAttackPlaySound`) and ends, else the position advances by `vel` (xyz, w kept). */
void BtlAttackType68Update(BtlAttack *self)
{
    if (self->age > 150) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        return;
    }
    BtlAttackSteerToTarget(20.0f, 150.0f, self, 1, NULL);
    BtlAttackCheckClash(self);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x8b, 3, 0, 0x31bf337e) != 0) {
        BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] += self->vel[0];
    self->pos[1] += self->vel[1];
    self->pos[2] += self->vel[2];
}

// bdc 0x0887fa7c BtlAttackType40Update
#include "bdc.h"

/* Per-frame handler of attack type 0x40 (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   homing shot. Ends once its age exceeds 90 frames. Frame 0: rises 50 units unless `paramF2` is
   non-zero, snaps onto the target (`BtlAttackSteerToTarget` speed 0, height 40 with turn rate
   1) and sets turn rate 0.08. Later frames, unless `BtlAttackResolveClash` (impact 0xb8)
   consumed it: homes at speed 35 with a 40-unit height offset, sweeps for hits along its velocity
   (`BtlAttackSweepHit`, hit id 0x63, kind 3) queuing impact 0xb8 at `g_btlAttackHitPoint`
   (`BtlAttackSetPendingHit`); otherwise its effect's `dir` takes the attack's `dir` and the
   position advances by `vel` (xyz, w kept). */

void BtlAttackType40Update(BtlAttack *self)
{
    if (0x5a < self->age) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        if (self->paramF2 == 0.0f) {
            self->pos[1] = self->pos[1] + 50.0f;
        }
        self->turnRate = 1.0f;
        BtlAttackSteerToTarget(0.0f, 40.0f, self, 1, NULL);
        self->turnRate = 0.0799999982f;
        return;
    }
    if (BtlAttackResolveClash(self, 0xb8) != 0) {
        return;
    }
    BtlAttackSteerToTarget(35.0f, 40.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 99, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xb8, &g_btlAttackHitPoint.x);
        return;
    }
    {
        GfxEffect *effect = (GfxEffect *)self->effect;
        effect->dir[0] = self->dir[0];
        effect->dir[1] = self->dir[1];
        effect->dir[2] = self->dir[2];
        effect->dir[3] = self->dir[3];
    }
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}

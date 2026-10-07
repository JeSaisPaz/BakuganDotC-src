// bdc 0x088822ec BtlAttackType74Update
#include "bdc.h"

/* Per-frame handler of attack type 0x74 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   on the first frame shifts the start position sideways by 40 along (-dir.z, dir.y, dir.x)
   (subtracted when the side parameter `paramF2` is negative, else added, also for NaN; pos.w
   kept), then moves it down 12 when |paramF2| < 50 or up 15 otherwise. Every frame runs
   `BtlAttackUpdateGuided``(0.3, paramF2, self, 0x97, 0x100, 0x1b)`; on the first frame with
   |paramF2| < 50 it then scales vel.xyz by 0.8 and sets vel.w to 0 (the bank's S713, stored by
   the `sv.q` of a three-lane `vscl.t` result). */

void BtlAttackType74Update(BtlAttack *self)
{
    float offset[3];

    if (self->age == 0) {
        offset[0] = -self->dir[2] * 40.0f;
        offset[1] = self->dir[1] * 40.0f;
        offset[2] = self->dir[0] * 40.0f;
        if (self->paramF2 < 0.0f) {
            self->pos[0] = self->pos[0] - offset[0];
            self->pos[1] = self->pos[1] - offset[1];
            self->pos[2] = self->pos[2] - offset[2];
        } else {
            self->pos[0] = self->pos[0] + offset[0];
            self->pos[1] = self->pos[1] + offset[1];
            self->pos[2] = self->pos[2] + offset[2];
        }
        if (ABS(self->paramF2) < 50.0f) {
            self->pos[1] = self->pos[1] - 12.0f;
        } else {
            self->pos[1] = self->pos[1] + 15.0f;
        }
    }
    BtlAttackUpdateGuided(0.300000012f, self->paramF2, self, 0x97, 0x100, 0x1b);
    if (self->age == 0 && ABS(self->paramF2) < 50.0f) {
        self->vel[0] = self->vel[0] * 0.800000012f;
        self->vel[1] = self->vel[1] * 0.800000012f;
        self->vel[2] = self->vel[2] * 0.800000012f;
        self->vel[3] = 0.0f;
    }
}

// bdc 0x088803c8 BtlAttackType2AUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x2a (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a sweeping beam. On the first frame (`age == 0`) it sets `turnRate` to the sweep acceleration,
   +0.00025 when the side variant `paramF2` truncates to non-zero and -0.00025 otherwise, and
   resets `paramF2` (from then on the sweep rate) to 0. On later frames it adds `turnRate` to the
   sweep rate, clamps it to [-0.01, 0.01] (a NaN sum becomes 0.01), stores it, and rotates `vel`
   about the Y axis by `rate` radians (VFPU `vrot.q` of rate * 2/π quarter turns; y and w kept). Every
   frame then runs `BtlAttackUpdateBeam``(9.0, self, 0xf0, 0xf1, 0xf2, 0x4d, 0x28)`. */
void BtlAttackType2AUpdate(BtlAttack *self)
{
    float rate;
    float c, sn, x, y, z;

    if (self->age == 0) {
        if ((s32)self->paramF2 == 0) {
            self->turnRate = -0.000250000012f;
        } else {
            self->turnRate = 0.000250000012f;
        }
        self->paramF2 = 0.0f;
    } else {
        rate = self->paramF2 + self->turnRate;
        if (!(rate <= 0.00999999978f)) {
            rate = 0.00999999978f;
        } else if (rate < -0.00999999978f) {
            rate = -0.00999999978f;
        }
        self->paramF2 = rate;
        /* vrot by rate * 2/pi quarter turns = rate radians; vel.y/w kept */
        c = __builtin_cosf(rate);
        sn = __builtin_sinf(rate);
        x = self->vel[0];
        y = self->vel[1];
        z = self->vel[2];
        self->vel[0] = x * c + y * 0.0f + z * -sn;
        self->vel[2] = x * sn + y * 0.0f + z * c;
    }
    BtlAttackUpdateBeam(9.0f, self, 0xf0, 0xf1, 0xf2, 0x4d, 0x28);
}

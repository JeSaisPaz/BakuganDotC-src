// bdc 0x0887fe44 BtlAttackType43Update
#include "bdc.h"

/* Per-frame handler of attack type 0x43 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   reads the side variant `paramF2` (`+0x10c`) first, runs `BtlAttackUpdateHoming``(self, 0x1b, 0x66,
   40.0)`, and when `age` (`+0xf4`) is still 0 sets the curve parameter `paramF0` (`+0x104`) to
   -0.628318548 (pi/5) when that side value is 0.0, else +0.628318548. */

void BtlAttackType43Update(BtlAttack *self)
{
    float side = self->paramF2;

    BtlAttackUpdateHoming(self, 0x1b, 0x66, 40.0f);
    if (self->age == 0) {
        if (side == 0.0f) {
            self->paramF0 = -0.628318548f; /* 0xbf20d97c */
        } else {
            self->paramF0 = 0.628318548f; /* 0x3f20d97c */
        }
    }
}

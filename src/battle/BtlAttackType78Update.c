// bdc 0x08882458 BtlAttackType78Update
#include "bdc.h"

/* Per-frame handler of attack type 0x78 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   while `age` is 0 (the first frame) lowers the position's Y (`pos[1]`) by 10, then runs the
   shared homing-bolt update `BtlAttackUpdateSeekerBolt` with argument 0x9b. */
void BtlAttackType78Update(BtlAttack *self)
{
    if (self->age == 0) {
        self->pos[1] = self->pos[1] - 10.0f;
    }
    BtlAttackUpdateSeekerBolt(self, 0x9b);
}

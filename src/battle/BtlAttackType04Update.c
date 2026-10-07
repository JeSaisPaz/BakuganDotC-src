// bdc 0x0887b768 BtlAttackType04Update
#include "bdc.h"

/* Per-frame handler of attack type 0x4 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   calls `BtlAttackUpdateHoming``(attack, hit id 0x1b, hit effect 0x27, speed 50.0)`. */

void BtlAttackType04Update(BtlAttack *self)
{
    BtlAttackUpdateHoming(self, 0x1b, 0x27, 50.0f);
}

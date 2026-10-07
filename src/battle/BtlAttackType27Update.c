// bdc 0x088801bc BtlAttackType27Update
#include "bdc.h"

/* Per-frame handler of attack type 0x27 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   calls `BtlAttackUpdateSideGuidedShot``(attack, 0x4a)`. */

void BtlAttackType27Update(BtlAttack *self)
{
    BtlAttackUpdateSideGuidedShot(self, 0x4a);
}

// bdc 0x0887baa8 BtlAttackType08Update
#include "bdc.h"

/* Per-frame handler of attack type 0x8 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   calls `BtlAttackUpdateStraightShot``(attack, 0x2b)`. */
void BtlAttackType08Update(BtlAttack *self)
{
    BtlAttackUpdateStraightShot(self, 0x2b);
}

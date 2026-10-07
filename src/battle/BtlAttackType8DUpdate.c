// bdc 0x08882cf0 BtlAttackType8DUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x8d (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs the shared straight shot `BtlAttackUpdateStraightShot` with hit id 0xb0. */
void BtlAttackType8DUpdate(BtlAttack *self)
{
    BtlAttackUpdateStraightShot(self, 0xb0);
}

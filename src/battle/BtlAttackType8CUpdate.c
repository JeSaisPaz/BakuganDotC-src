// bdc 0x08882cd4 BtlAttackType8CUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x8c (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   calls `BtlAttackUpdateSideGuidedShot``(attack, 0xaf)`. */
void BtlAttackType8CUpdate(BtlAttack *self)
{
    BtlAttackUpdateSideGuidedShot(self, 0xaf);
}

// bdc 0x08880b0c BtlAttackType5FUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x5f (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs the shared trailed homing shot `BtlAttackUpdateTrailedHomingShot` with hit id 0x82. */
void BtlAttackType5FUpdate(BtlAttack *self)
{
    BtlAttackUpdateTrailedHomingShot(self, 0x82);
}

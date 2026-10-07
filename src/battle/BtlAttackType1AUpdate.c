// bdc 0x0887d2f4 BtlAttackType1AUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x1a (entry 26 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs `BtlAttackUpdateTrailedHomingShot` with hit id 0x3d. */
void BtlAttackType1AUpdate(BtlAttack *self)
{
    BtlAttackUpdateTrailedHomingShot(self, 0x3d);
}

// bdc 0x0887d2bc BtlAttackType18Update
#include "bdc.h"

/* Per-frame handler of attack type 0x18 (entry 24 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs `BtlAttackUpdateTrailedHomingShot` with hit id 0x3b. */
void BtlAttackType18Update(BtlAttack *self)
{
    BtlAttackUpdateTrailedHomingShot(self, 0x3b);
}

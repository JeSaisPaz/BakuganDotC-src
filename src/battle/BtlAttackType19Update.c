// bdc 0x0887d2d8 BtlAttackType19Update
#include "bdc.h"

/* Per-frame handler of attack type 0x19 (entry 25 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs `BtlAttackUpdateTrailShot` with hit kind 0x3c. */
void BtlAttackType19Update(BtlAttack *self)
{
    BtlAttackUpdateTrailShot(self, 0x3c);
}

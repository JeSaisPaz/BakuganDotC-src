// bdc 0x0887db38 BtlAttackType1DUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x1d (entry 29 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs `BtlAttackUpdateArcShot` with hit id 0x40. */
void BtlAttackType1DUpdate(BtlAttack *self)
{
    BtlAttackUpdateArcShot(self, 0x40);
}

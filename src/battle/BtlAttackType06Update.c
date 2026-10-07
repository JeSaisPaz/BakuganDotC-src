// bdc 0x0887b8c8 BtlAttackType06Update
#include "bdc.h"

/* Per-frame handler of attack type 0x06 (entry 6 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs `BtlAttackUpdateHomingShotB` with hit id 0x29. */
void BtlAttackType06Update(BtlAttack *self)
{
    BtlAttackUpdateHomingShotB(self, 0x29);
}

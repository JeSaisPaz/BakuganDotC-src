// bdc 0x0887b790 BtlAttackType05Update
#include "bdc.h"

/* Per-frame handler of attack type 0x5 (entry 5 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs `BtlAttackUpdateSeekerBolt` with hit kind 0x28. */
void BtlAttackType05Update(BtlAttack *self)
{
    BtlAttackUpdateSeekerBolt(self, 0x28);
}

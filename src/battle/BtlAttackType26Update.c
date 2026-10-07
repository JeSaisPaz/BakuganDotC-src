// bdc 0x088800fc BtlAttackType26Update
#include "bdc.h"

/* Per-frame handler of attack type 0x26 (entry 38 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs `BtlAttackUpdateRisingHomingShot` with hit id 0x49. */
void BtlAttackType26Update(BtlAttack *self)
{
    BtlAttackUpdateRisingHomingShot(self, 0x49);
}

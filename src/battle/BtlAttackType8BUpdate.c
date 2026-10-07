// bdc 0x08882cb8 BtlAttackType8BUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x8b (slot 2 * 0x8b + 1 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs the shared rising homing shot update with hit id 0xae,
   `BtlAttackUpdateRisingHomingShot``(self, 0xae)`. */
void BtlAttackType8BUpdate(BtlAttack *self)
{
    BtlAttackUpdateRisingHomingShot(self, 0xae);
}

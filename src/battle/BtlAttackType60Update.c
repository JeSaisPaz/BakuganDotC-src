// bdc 0x08880b28 BtlAttackType60Update
#include "bdc.h"

/* Per-frame handler of attack type 0x60 (slot 0x60 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): calls `BtlAttackUpdateTrailShot``(attack,0x83)`. */
void BtlAttackType60Update(BtlAttack *self)
{
    BtlAttackUpdateTrailShot(self, 0x83);
}

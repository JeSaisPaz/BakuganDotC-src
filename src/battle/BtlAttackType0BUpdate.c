// bdc 0x0887bbe4 BtlAttackType0BUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0xb (entry 0xb of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs the shared beam `BtlAttackUpdateBeam` with width 20.0, effect 0x20
   and the ids 0x21, 0x22, 0x2e, 0x28. */
void BtlAttackType0BUpdate(BtlAttack *self)
{
    BtlAttackUpdateBeam(20.0f, self, 0x20, 0x21, 0x22, 0x2e, 0x28);
}

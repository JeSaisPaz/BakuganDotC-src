// bdc 0x0887ce94 BtlAttackType14Update
#include "bdc.h"

/* Per-frame handler of attack type 0x14 (slot 0x14 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): calls `BtlAttackUpdateGuided``(0.1,300.0,attack,0x37,0x25,0x1b)`. */
void BtlAttackType14Update(BtlAttack *self)
{
    BtlAttackUpdateGuided(0.100000001f, 300.0f, self, 0x37, 0x25, 0x1b);
}

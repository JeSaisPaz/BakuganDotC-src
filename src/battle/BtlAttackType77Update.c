// bdc 0x08882430 BtlAttackType77Update
#include "bdc.h"

/* Per-frame handler of attack type 0x77 (entry 119 of `g_btlAttackTypeHandlers`, run by
   `BtlAttackUpdate`): runs the shared homing projectile `BtlAttackUpdateHoming` with hit id
   0x1b, hit effect 0x9a and speed 50.0. */
void BtlAttackType77Update(BtlAttack *self)
{
    BtlAttackUpdateHoming(self, 0x1b, 0x9a, 50.0f);
}

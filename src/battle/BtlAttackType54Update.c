// bdc 0x08884610 BtlAttackType54Update
#include "bdc.h"

/* Per-frame handler of attack type 0x54 (`g_btlAttackTypeHandlers`, run by `BtlAttackUpdate`):
   a simple projectile; calls `BtlAttackUpdateProjectile` with speed 20, max speed 80, hit kind
   0x77, hit effect 0x51 on the unit effect manager `g_btlUnitEffectMgr`, no trail effect (-1),
   lifetime 90 frames, sound 0x200096, `flagA` 0 and `flagB` 1. */
void BtlAttackType54Update(BtlAttack *self)
{
    BtlAttackUpdateProjectile(20.0f, 80.0f, self, 0x77, 0x51, g_btlUnitEffectMgr, -1, 90, 0,
                              0x200096, 1);
}

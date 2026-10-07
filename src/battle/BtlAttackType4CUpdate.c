// bdc 0x0888424c BtlAttackType4CUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x4c (`g_btlAttackTypeHandlers`, run by `BtlAttackUpdate`):
   a simple projectile; calls `BtlAttackUpdateProjectile` with speed 25, max speed 80, hit kind
   0x6f, hit effect 0x49 on the unit effect manager `g_btlUnitEffectMgr`, trail effect 0xf0,
   lifetime 60 frames, sound 0x200096, `flagA` 1 and `flagB` 1. */
void BtlAttackType4CUpdate(BtlAttack *self)
{
    BtlAttackUpdateProjectile(25.0f, 80.0f, self, 0x6f, 0x49, g_btlUnitEffectMgr, 0xf0, 60, 1,
                              0x200096, 1);
}
